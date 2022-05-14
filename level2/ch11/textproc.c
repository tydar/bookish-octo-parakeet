#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

typedef struct tp_text tp_text;
struct tp_text {
	size_t len;
	char* content;
	tp_text* prev;
	tp_text* next;
};

tp_text* tp_new_text(char* content, size_t len) {
	tp_text* ptr = malloc(sizeof(tp_text));
	ptr->len = len;
	ptr->content = content;
	ptr->prev = 0;
	ptr->next = 0;

	return ptr;
}

void tp_print_struct(tp_text* text) {
	if (!text) { puts("Null ptr"); return; }
	printf("Ptr: %p\nCalc'd len: %zu\n{\n\tLen: %zu,\n\tContent:%s,\n\tPrev: %p,\n\tNext: %p\n}\n",
			(void*)text,
			strlen(text->content),
			text->len,
			text->content,
			(void*)text->prev,
			(void*)text->next);
}

tp_text* tp_insert_after(tp_text* insert, tp_text* after) {
	if (!insert || !after) return 0;
	
	insert->prev = after;
	insert->next = after->next;
	after->next = insert;

	return insert;
}

tp_text* tp_insert_before(tp_text* insert, tp_text* before) {
	if (!insert || !before) return 0;
	
	insert->next = before;
	insert->prev = before->prev;
	before->prev = insert;

	return insert;
}


tp_text* tp_split_at(tp_text* text, size_t index) {
	if (index > text->len - 1) return 0;
	if (index == 0) return text;



	char* str1 = malloc(index + 1);
	char* str2 = text->content + index - 1;

	str1 = strncpy(str1, text->content, index);
	str1[index-1] = '\0';

	tp_text* new_text = tp_new_text(str2, text->len - index +  1);

	text->content = str1;
	text->len = index-1;

	new_text = tp_insert_after(new_text, text);

	return new_text;
}

tp_text* tp_join_next(tp_text* first) {
	if (!first || !first->next) { puts("Null ptr!"); return 0; }

	char* newStr = malloc(first->len + first->next->len);
	if(!strcpy(newStr, first->content)) { perror("strcpy: "); errno = 0; return 0; }
	if(!strcat(newStr, first->next->content)) { perror("strcat: "); errno = 0; return 0; }

	first->content = newStr;
	first->len = first->len + first->next->len;

	tp_text* oldNext = first->next;

	first->next = first->next->next;

	free(oldNext);

	return first;
}

void tp_print_paras(tp_text* head) {
	if(!head) { puts("Null ptr!"); return; }

	while(head) {
		puts(head->content);
		head = head->next;
	}
}

int main(void) {
	tp_text* t1 = tp_new_text("Hey there!", 10);
	tp_text* t2 = tp_new_text(" What?", 6);
	t1 = tp_insert_before(t1, t2);

	tp_print_paras(t1);

	return EXIT_SUCCESS;
}

