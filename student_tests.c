/* Name: Shyam Polisetty
UID: 119822999
Directory ID: spoliset*/

#include <stdio.h>
#include <stdlib.h>
#include "event.h"
#include "calendar.h"

/*****************************************************/
/* In this file you will provide tests for your      */
/* calendar application.  Each test should be named  */
/* test1(), test2(), etc. Each test must have a      */
/* brief description of what it is testing (this     */
/* description is important).                        */
/*                                                   */
/* You can tell whether any test failed if after     */
/* executing the students tests, you executed        */
/* "echo $?" on the command line and you get a value */
/* other than 0.  "echo $?" prints the status of     */
/* the last command executed by the shell.           */ 
/*                                                   */
/* Notice that main just calls test1(), test2(), etc.*/
/* and once one fails, the program eventually        */
/* return EXIT_FAILURE; otherwise EXIT_SUCCESS will  */
/* be returned.                                      */
/*****************************************************/

static int comp_minutes(const void *ptr1, const void *ptr2) {
   return ((Event *)ptr1)->duration_minutes - ((Event *)ptr2)->duration_minutes;
}

/* This test checks  init_calendar, print_calendar and destroy_calendar */
static int test1() {
   int days = 7;
   Calendar *calendar;

   printf("------------------------------------------------------");

   if (init_calendar("Spr", days, comp_minutes, NULL, &calendar) == SUCCESS) {
      if (print_calendar(calendar, stdout, 1) == SUCCESS) {
         return destroy_calendar(calendar);
      }
   }
    
   return FAILURE;
}

/* This test checks init_calendar, add_event, print_calendar, and destroy_calendar */
static int test2() {
    int days = 7;
    Calendar *calendar;

    printf("------------------------------------------------------");

    if (init_calendar("WeekPlan", days, comp_minutes, NULL, &calendar) == SUCCESS) {
        add_event(calendar, "Meeting", 900, 60, NULL, 1);
        add_event(calendar, "Lunch", 1200, 30, NULL, 1);
        add_event(calendar, "Gym", 1800, 60, NULL, 2);

        if (print_calendar(calendar, stdout, 1) == SUCCESS) {
            return destroy_calendar(calendar);
        }
    }

    return FAILURE;
}

/* This test checks the addition of duplicate events and the proper handling of removal */
static int test3() {
    int days = 7;
    Calendar *calendar;

    printf("------------------------------------------------------");

    if (init_calendar("Test Calendar", days, comp_minutes, NULL, &calendar) == SUCCESS) {
        add_event(calendar, "Workshop", 1000, 120, NULL, 3);
        
        if (add_event(calendar, "Workshop", 1000, 120, NULL, 3) == FAILURE) {
            printf("Duplicate event not allowed.\n");
        }

        if (remove_event(calendar, "Workshop") == SUCCESS) {
            printf("Event removed successfully.\n");
        }

        if (remove_event(calendar, "Workshop") == FAILURE) {
            printf("Event already removed.\n");
        }

        return destroy_calendar(calendar);
    }

    return FAILURE;
}

/* This test checks clear_day and total_events */
static int test4() {
    int days = 7;
    Calendar *calendar;

    printf("------------------------------------------------------");

    if (init_calendar("Weekly Schedule", days, comp_minutes, NULL, &calendar) == SUCCESS) {
        add_event(calendar, "Breakfast", 800, 30, NULL, 1);
        add_event(calendar, "Work", 900, 480, NULL, 1);
        add_event(calendar, "Dinner", 1800, 60, NULL, 1);

        if (clear_day(calendar, 1) == SUCCESS) {
            printf("Day cleared! Total events: %d\n", calendar->total_events);
        }

        return destroy_calendar(calendar);
    }

    return FAILURE;
}

/* This test checks find_event and get_event_info */
static int test5() {
    int days = 7;
    Calendar *calendar;
    Event *found_event;
    void *info;

    printf("------------------------------------------------------");

    if (init_calendar("Info Check", days, comp_minutes, NULL, &calendar) == SUCCESS) {
        add_event(calendar, "RA Meeting", 1100, 90, NULL, 4);
        
        if (find_event(calendar, "RA Meeting", &found_event) == SUCCESS) {
            printf("Found event: %s\n", found_event->name);
        }

        info = get_event_info(calendar, "RA Meeting");
        if (info != NULL) {
            printf("Event info received!\n");
        }

        return destroy_calendar(calendar);
    }

    return FAILURE;
}

static int test6() {
    int days = 7;
    Calendar *calendar;

    printf("------------------------------------------------------");

    if (init_calendar("WeekPlan", days, comp_minutes, NULL, &calendar) == SUCCESS) {
        add_event(calendar, "Meeting", 900, 60, NULL, 1);
        add_event(calendar, "Lunch", 1200, 30, NULL, 1);
        add_event(calendar, "Gym", 1800, 60, NULL, 2);

        if (print_calendar(calendar, stdout, 1) == SUCCESS) {
            return destroy_calendar(calendar);
        }
    }

    return FAILURE;
}

int main() {
   int result = SUCCESS;

   if (test1() == FAILURE) result = FAILURE;
   if (test2() == FAILURE) result = FAILURE;
   if (test3() == FAILURE) result = FAILURE;
   if (test4() == FAILURE) result = FAILURE;
   if (test5() == FAILURE) result = FAILURE;

   if (result == FAILURE) {
      exit(EXIT_FAILURE);
   }

   return EXIT_SUCCESS;
}
