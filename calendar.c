/* Name: Shyam Polisetty
UID: 119822999
Directory ID: spoliset*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "event.h"
#include "calendar.h"

int init_calendar(const char *name, int days, int (*comp_func) (const void *ptr1, const void *ptr2), 
void (*free_info_func) (void *ptr), Calendar ** calendar) {

    /*null checks*/
    if(name == NULL || calendar == NULL || days < 1) {
        return FAILURE;
    }

    /*Allocate memory for the calendar, using sizeof to get how much we space we would need*/
    *calendar = (Calendar *)malloc(sizeof(Calendar));
    if(*calendar == NULL) {
        return FAILURE;
    }

    /*Allocate memory for the String name, add one for the null terminator*/
    (*calendar)->name = (char *)malloc(strlen(name) + 1);
    if((*calendar)-> name == NULL) {
        free((*calendar));
        return FAILURE;
    } else {
        strcpy((*calendar)->name, name);
    }

    /*Allocate memory for events. Check for null and free previously allocated memory.
    Don't need to free (*calendar)->events because if calloc fails, no memory is allocated*/
    (*calendar)->events = (Event **)calloc(days, sizeof(Event *));
    if((*calendar)->events == NULL) {
        free((*calendar)->name);
        free(*calendar);
        return FAILURE;
    }

    /*Initialize all the data fields*/
    (*calendar)->days = days;
    (*calendar)->total_events = 0;
    (*calendar)->comp_func = comp_func;
    (*calendar)->free_info_func = free_info_func;

    return SUCCESS;
}

int print_calendar(Calendar *calendar, FILE *output_stream, int print_all) {
    int i; 
    Event *print_event;

    /*Null checks*/
    if(calendar == NULL || output_stream == NULL) {
        return FAILURE;
    }

    /*if print_all, then print the calendar's name, days and total events into output_stream*/
    if(print_all) {
        fprintf(output_stream, "Calendar's Name: \"%s\"\n", calendar->name);
        fprintf(output_stream, "Days: %d\n", calendar->days);
        fprintf(output_stream, "Total Events: %d\n", calendar->total_events);
    }

    /*printing the header, which will ALWAYS be printed*/
    fprintf(output_stream, "\n**** Events ****\n");

    /*printing the details of each event which will always be printed.
    using for loop to cycle through days in a calendar (the 'i' for loop)
    then using for loop to cycle through each days events in a calendar (the while loop)
    and then checking if the event at that index is null or not*/
    if(calendar->total_events > 0) {
        for(i = 0; i < calendar->days; i++) {
            fprintf(output_stream, "Day %d\n", i + 1);
            print_event = calendar->events[i];
            while(print_event != NULL) {
                fprintf(output_stream, "Event's Name: \"%s\", ", print_event->name);
                fprintf(output_stream, "Start_time: %d, ", print_event->start_time);
                fprintf(output_stream, "Duration: %d\n", print_event->duration_minutes);
                print_event = print_event->next;
            }
        }
    }

    return SUCCESS;    
}

int add_event(Calendar *calendar, const char *name, int start_time,
int duration_minutes, void *info, int day) {
    Event *event, *curr, *previous = NULL;

    /*Null Checks*/
    if(calendar == NULL || name == NULL) {
        return FAILURE;
    }

    /*Making sure the start_time, duration_minutes and days parameters are valid*/
    if(start_time < 0 || start_time > 2400 || duration_minutes <= 0 || day < 1 || day > calendar->days) {
        return FAILURE;
    }

    /*Checking if event already exists by going through total_events and using strcmp*/
    curr = calendar->events[day - 1];
    while(curr != NULL) {
        if(strcmp(curr->name, name) == 0) {
            return FAILURE;
        }
        curr = curr->next;
    }
    

    /*Allocate memory for the event, using sizeof to get how much we space we would need*/
    event = (Event *)malloc(sizeof(Event));
    if(event == NULL) {
        return FAILURE;
    }

    /*Allocate memory for the String name, add one for the null terminator, exactly like how we did for calendar*/
    event->name = (char *)malloc(strlen(name) + 1);
    if(event->name == NULL) {
        free(event);
        return FAILURE;
    }

    /*Initialize rest of the fields*/
    strcpy(event->name, name);
    event->start_time = start_time;
    event->duration_minutes = duration_minutes;
    event->info = info;
    event->next = NULL;

    /*Finding where to insert the event using the comp_func and then inserting it*/
    curr = calendar->events[day - 1];
    while(curr != NULL && calendar->comp_func(curr, event) < 0) {
        previous = curr;
        curr = curr->next;
    }

    /**/
    if(previous == NULL) {
        event->next = calendar->events[day - 1];
        calendar->events[day - 1] = event;
    } else {
        previous->next = event;
        event->next = curr;
    }

    /*Incrementing total events by one*/
    calendar->total_events++;

    return SUCCESS;
}

int destroy_calendar(Calendar *calendar) {
    int i;
    Event *current;
    Event *future;
    
    /*Null Checks*/
    if(calendar == NULL) {
        return FAILURE;
    }

    /* Going through every day in the calendar, then every event on thar day*/
    for(i = 0; i < calendar->days; i++) {
        current = calendar->events[i];
        while(current != NULL) {
            future = current->next;

            /*Check for free_info_func and use it on info*/
            if(calendar->free_info_func != NULL && current->info != NULL) {
                calendar->free_info_func(current->info);
            }

            

            /*Free the event name and event*/
            free(current->name);
            free(current);

            /*Parse onto the next event*/
            current = future;
        }
    }

    /*Free calendar name and event array and calendar itself*/
    free(calendar->name);
    free(calendar->events);
    free(calendar);

    return SUCCESS;
} 

int find_event(Calendar *calendar, const char *name, Event **event) {
    int i;
    Event *current;

    /*Null Checks*/
    if(calendar == NULL || name == NULL) {
        return FAILURE;
    }

    /*Checking if event null, and if it isn't then change it to NULL*/
    if(event != NULL) {
        *event = NULL;
    } 

    /*Looping through each day and assigning calender->days[i] to current*/
    for(i = 0; i < calendar->days; i++) {
        current = calendar->events[i];

        /*Going through each event in a day and checking if its name matches with that of the name parameter
        if it does, then set *event to current and return success, if not, iterate over the event using next*/
        while(current != NULL) {
            if(strcmp(current->name, name) == 0) {
                if(event != NULL) {
                    *event = current;
                    return SUCCESS;
                }
            }
            current = current->next;
        }
    }

    return FAILURE;

}

int find_event_in_day(Calendar *calendar, const char *name, int day, Event **event) {
    Event *current;

    /*Null Checks*/
    if(calendar == NULL || name == NULL) {
        return FAILURE;
    }

    /*Check if day is valid*/
    if(day < 1 || day > calendar->days) {
        return FAILURE;
    }

    /*Checking if event null, and if it isn't then change it to NULL*/
    if(event != NULL) {
        *event = NULL;
    }

    /*Get the specified day (-1 because days start at 0)*/
    current = calendar->events[day - 1];

    /*Going through each event in a day and checking if its name matches with that of the name parameter
    if it does, then set *event to current and return success, if not, iterate over the event using next*/
    while(current != NULL) {
        if(strcmp(current->name, name) == 0) {
            if(event != NULL) {
                *event = current;
                current = current->next;
            }
            return SUCCESS;
        }
    }

    return FAILURE;
}

int remove_event(Calendar *calendar, const char *name) {
    int i;
    Event *current, *temp, *previous = NULL;

    /*Null checks*/
    if(calendar == NULL || name == NULL) {
        return FAILURE;
    }

    /*Loop through each day in the calendar*/
    for(i = 0; i < calendar->days; i++) {
        /*Set current equal to calendar->events[i]*/
        current = calendar->events[i];
        previous = NULL;

        /*Loop through each event in the day and use strcmp to see if the event name equals the parameter*/
        while(current != NULL) {
            if(strcmp(current->name, name) == 0) {
                temp = current->next;
                /*If event isn't first, then just link the previous to the next event, skipping current*/
                /*Else, if it is first, just change the linked list head to the next event*/
                if(previous == NULL) {
                    calendar->events[i] = current->next;
                } else {
                    previous->next = current->next;
                }

                /* Check for free_info_func and use it*/
                if(current->info != NULL && calendar->free_info_func != NULL) {
                    calendar->free_info_func(current->info);
                }

                /*Free current and its fields*/
                free(current->name);
                free(current);

                current = temp;

                /*Decrement calendar->total_events*/
                calendar->total_events--;

                return SUCCESS;
            } else {
                /*Update current and previous*/
                previous = current;
                current = current->next;
            }
        }
    }

    return FAILURE;
}

void *get_event_info(Calendar *calendar, const char *name) {
    int i;
    Event *current;

    /*No Null Checks Needed*/

    /*Looping through all calendar days*/
    for(i = 0; i < calendar->days; i++) {
        /*Initializing current*/
        current = calendar->events[i];

        /*Looping through all events on a day and using strcmp to see if name matches*/
        /*If it does, return current->info*/
        while(current != NULL) {
            if(strcmp(current->name, name) == 0) {
                return current->info;
            }
            /*traversing day using next*/
            current = current->next;
        }
    }

    return NULL;
}

int clear_calendar(Calendar *calendar) {
    /*Very Similar to destroy calendar*/
    int i;
    Event *current, *future;

    /*Null checks*/
    if(calendar == NULL) {
        return FAILURE;
    }

    /*looping through all the calendar days*/
    for(i = 0; i < calendar->days; i++) {
        /*initializing current*/
        current = calendar->events[i];

        /*looping through events in a day*/
        while(current != NULL) {

            future = current->next;

            /*Check for info and free_info_function to clear info, then use it*/
            if(current->info != NULL && calendar->free_info_func != NULL) {
                calendar->free_info_func(current->info);
            }
            
            /*free current and its fields*/
            free(current->name);
            free(current);

            /*Moving to the next event by using the previously saved future since current is now now gone
            and we can't do current->next*/
            current = future;
        }

        calendar->events[i] = NULL;
    }

    /*Update calendar->total_events*/
    calendar->total_events = 0;
    return SUCCESS;
}

int clear_day(Calendar *calendar, int day) {
    /*Similar to find event in day*/
    Event *current, *future;

    /*Null Checks*/
    if(calendar == NULL || day < 1 || day > calendar->days) {
        return FAILURE;
    }

    /*Get the specified day (-1 because days start at 0)*/
    current = calendar->events[day - 1];

    /*Loop through all events in the day*/
    while(current != NULL) {
        future = current->next;

        /*Check for info and free_info_function to clear info, then use it*/
        if(current->info != NULL && calendar->free_info_func != NULL) {
            calendar->free_info_func(current->info);
        }

        /*Free current and its fields*/
        free(current->name);
        free(current);

        /*Moving to the next event by using the previously saved future since current is now now gone
        and we can't do current->next*/
        current = future;

        /*Updating total events*/
        calendar->total_events--;
    }

    calendar->events[day - 1] = NULL;
    return SUCCESS;
} 