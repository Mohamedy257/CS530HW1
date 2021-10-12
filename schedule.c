/* Fill in your Name and GNumber in the following two comment fields
 * Name:Mohamed Yousif Mohamed
 * GNumber:G01303134
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "clock.h"
#include "structs.h"
#include "constants.h"

/* Schedule Count
 * - Return the number of Processes in the list.
 */
int schedule_count(Process *list) {
  
	int count =0;
	Process *iterator = list;

	while(iterator!=NULL)
	{
		++count;
		iterator = iterator->next;
	}

  return count;
}

/* Schedule Insert
 * - Insert the Process into the List with the Following Constraints
 *   list is a pointer to the head pointer for a singly linked list.
 *   It may begin as NULL
 *   1) Insert in the order of Ascending PIDs
 *   (eg. 1, 13, 14, 20, 32, 55)
 * Remember:wq: to set list to the new head if it changes!
 */
void schedule_insert(Process **list, Process *node) {

	if (*list == NULL || (*list)->pid >= node->pid)
	{
		node->next = *list;
		*list = node;
	}
	else
	{
		Process *current = *list; //current node initialised to head
		while (current->next != NULL && current->next->pid < node->pid)
		{
			current = current->next;
		}
		node->next = current->next;
		current->next = node;
	}
}

/* Schedule Terminate
 * - Unallocate the Memory for the given Node
 */
void schedule_terminate(Process *node) {	
	if (node == NULL)
	{
		return;
	}

	free(node);
}

/* Schedule Generate
 * - Allocate the Memory and Create the Process from the given Variables
 *   Remember to initialize all values, including next!
 */
Process *schedule_generate(const char *name, int pid, int time_remaining, int time_last_run) {
 
 Process *new =malloc(sizeof(Process));
 if (new == NULL)
 {
	 return NULL;
 }

 new->pid = pid;
 strncpy(new->name, name, strlen(name) + 1);
 new->time_remaining = time_remaining;
 new->time_last_run = time_last_run;
 new->next = NULL;
 return new;
}

//Run the logic to select the process based on the remaining time and then override that if it figured that the
//starvation time is greator the the remaining time
void GetSelectedProcess(Process **head,Process **selectedProcess,Process **previousProcess)
{
	Process *iterator = *head;
	Process *previousIterator = NULL;

	int lowestTime = (*head)->time_remaining;
	int LowestPid = (*head)->pid;
	int selectByStarvation= 0;

	for(;iterator != NULL;iterator = iterator->next)
	{
		if (iterator->time_remaining < lowestTime &&!selectByStarvation)
		{
			LowestPid = iterator->pid;
			lowestTime =iterator->time_remaining;
			*selectedProcess = iterator;
			*previousProcess =previousIterator;

		}

		else if (iterator->time_remaining == lowestTime &&!selectByStarvation)
		{
			if (iterator->pid < LowestPid)
			{
				*selectedProcess = iterator;
				*previousProcess =previousIterator;
			}
		}

		if ((clock_get_time()-(iterator->time_last_run)) >= TIME_STARVATION)
		{
			if (selectByStarvation == 1)
			{
				if (iterator->pid < (*selectedProcess)->pid)
					*selectedProcess = iterator;
			}
			else
			{
				selectByStarvation = 1;
				*selectedProcess = iterator;
				*previousProcess =previousIterator;
			}
		}

		previousIterator = iterator;
	}
	
}

//Delete selected Process by rearranging the previous and next element on the list
//if the deleted element is the head then we point to a new head which is head next node
void DeleteSelectedProcess(Process **head,Process **selectedProcess,Process **previousProcess)
{
	if (*selectedProcess == 0)
	{
		*selectedProcess = *head;
		*head= (*head)->next;
	}

	else
	{
		if(*previousProcess == 0)
		{
			*head = (*selectedProcess)->next;
		}
		else
		{
			(*previousProcess)->next = (*selectedProcess)->next;
		}
	}
}

/* Schedule Select
 * - Select the next Process to be run using the following conditions:
 *   1) The Process with the lowest time_remaining should run next.
 *   - If there are any ties, select the Process with the lowest PID
 *   2) If there is a Process that has not run in >= TIME_STARVATION, then
 *      select that one instead.
 *      (Use the function clock_get_time() to get the current time)
 *   - If there are any ties, select the Process with the lowest PID
 *   3) If the list is NULL, return NULL
 * - Remove the selected Process from the list
 * - Return the selected Process
 */
Process *schedule_select(Process **list) {
	Process *selectedProcess = NULL;
	Process *previousProcess = NULL;

	if (*list == 0)
	{
		return NULL;
	}

	GetSelectedProcess(list,&selectedProcess,&previousProcess);
	DeleteSelectedProcess(list,&selectedProcess,&previousProcess);
	return selectedProcess;
}
