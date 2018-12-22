#include "dictionary.h"
#include "aiv_unit_test.h"

int tests_succeded = 0;
int tests_failed = 0;
int tests_executed = 0;

TEST(dumb){
    ASSERT_THAT(1);
}

TEST(init_dict){
    dictionary_t* dict = new_dictionary();
    ASSERT_THAT(dict != NULL);
    ASSERT_THAT(dict->entries == NULL);
    // ASSERT_THAT(dict->size == 10);
}

TEST(new_entry){
    key_value_t* entry = new_key_value((void *)"gino", (void *)1938);
    ASSERT_THAT((char*)entry->key == "gino");
    ASSERT_THAT((int)entry->value == 1938);
    ASSERT_THAT(entry->next == NULL);
}

TEST(add_entry){
    dictionary_t* dict = new_dictionary();
    key_value_t* entry = new_key_value((void *)"gino", (void *)1938);
    add_key_value(dict, entry);
    ASSERT_THAT(dict->entries != NULL);
    ASSERT_THAT((char*)dict->entries->key == "gino");
    ASSERT_THAT((int)dict->entries->value == 1938);
}

TEST(add_multiple_entries){
    dictionary_t* dict = new_dictionary();
    key_value_t* entry1 = new_key_value((void *)"gino", (void *)1938);
    add_key_value(dict, entry1);
    key_value_t* entry2 = new_key_value((void *)"pino", (void *)748);
    add_key_value(dict, entry2);
    ASSERT_THAT(dict->entries != NULL);
    ASSERT_THAT((char*)dict->entries->key == "gino");
    ASSERT_THAT((int)dict->entries->value == 1938);
}

TEST(get_value){
    dictionary_t* dict = new_dictionary();
    key_value_t* entry = new_key_value((void *)"gino", (void *)1938);
    add_key_value(dict, entry);
    void* key_value = get_value(dict, (void*)"gino");
    ASSERT_THAT((int)key_value == 1938);
}

TEST(get_multiple_value){
    dictionary_t* dict = new_dictionary();
    key_value_t* entry1 = new_key_value((void *)"gino", (void *)1938);
    add_key_value(dict, entry1);
    void* gino_value = get_value(dict, (void*)"gino");
    key_value_t* entry2 = new_key_value((void *)"motumbo", (void *)4664);
    add_key_value(dict, entry2);
    void* motumbo_value = get_value(dict, (void*)"motumbo");
    ASSERT_THAT((int)gino_value == 1938);
    ASSERT_THAT((int)motumbo_value == 4664);
}

int main(int argc, char **argv)
{
    RUN_TEST(dumb);
    RUN_TEST(init_dict);
    RUN_TEST(new_entry);
    RUN_TEST(add_entry);
    RUN_TEST(add_multiple_entries);
    RUN_TEST(get_value);
    RUN_TEST(get_multiple_value);
    PRINT_TEST_RESULTS();
    return 0;
}
