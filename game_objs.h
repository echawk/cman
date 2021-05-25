/*
	Types to define:
	Enemy     (Entity)
	Player    (Entity)
	PowerPill (Entity)
	Wall      (Entity)
	An Entity just has an x and a y value associated with it, as well as a character for printing.
	Enemies also have a char to denote their 'type' whether it is a enemy, player, powerpill, or wall


	Enemy List
	Wall List
	PowerPill List

	These are ment to be a list consisting of the specific type. Technically, we only need one type
	defintition because they are all lists of entities.

	Currently, the thought is to use a singly linked list to implent the list type we are going for,
	since the size is likely to be quite small (less than 1000), and the ease of implementation.
*/

typedef struct entity {
	int x;
	int y;
	char icon;
	char type;
} entity_t;


typedef struct entity_list_node {
	entity_t *next;
	entity_t value;
} entity_list_node_t;


typedef struct entity_list {
	entity_list_node_t *head;
} entity_list_t;
