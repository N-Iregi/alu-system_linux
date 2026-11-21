#include <stdlib.h>
#include "multithreading.h"

/**
 * create_task - creates a task
 * @entry: task entry function
 * @param: parameter passed to entry
 *
 * Return: pointer to the task, or NULL on failure
 */
task_t *create_task(task_entry_t entry, void *param)
{
	task_t *task;

	if (!entry)
		return (NULL);
	task = malloc(sizeof(*task));
	if (!task)
		return (NULL);
	task->entry = entry;
	task->param = param;
	task->status = PENDING;
	task->result = NULL;
	pthread_mutex_init(&task->lock, NULL);
	return (task);
}

/**
 * destroy_task - destroys a task and its result
 * @task: task to destroy
 *
 * The result is assumed to be a list_t of malloc'd items.
 */
void destroy_task(task_t *task)
{
	if (!task)
		return;
	if (task->result)
	{
		list_destroy((list_t *)task->result, free);
		free(task->result);
	}
	pthread_mutex_destroy(&task->lock);
	free(task);
}

/**
 * run_task - runs one task and reports start and completion
 * @task: task to run
 * @id: index of the task in the list
 */
static void run_task(task_t *task, size_t id)
{
	tprintf("[%02lu] Started\n", (unsigned long)id);
	task->result = task->entry(task->param);
	pthread_mutex_lock(&task->lock);
	task->status = task->result ? SUCCESS : FAILURE;
	pthread_mutex_unlock(&task->lock);
	tprintf("[%02lu] %s\n", (unsigned long)id,
		task->result ? "Success" : "Failure");
}

/**
 * exec_tasks - thread entry, executes every pending task of the list
 * @tasks: list of tasks
 *
 * Return: NULL
 */
void *exec_tasks(list_t const *tasks)
{
	node_t *node;
	task_t *task;
	size_t id = 0;
	int mine;

	if (!tasks)
		return (NULL);
	for (node = tasks->head; node; node = node->next, id++)
	{
		task = (task_t *)node->content;
		pthread_mutex_lock(&task->lock);
		mine = task->status == PENDING;
		if (mine)
			task->status = STARTED;
		pthread_mutex_unlock(&task->lock);
		if (mine)
			run_task(task, id);
	}
	return (NULL);
}
