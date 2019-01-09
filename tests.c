#include "dictionary.h"
#include "hash.h"
#include "aiv_unit_test.h"
#include "asm_operation.h"

int tests_succeded = 0;
int tests_failed = 0;
int tests_executed = 0;

typedef struct random_struct{
    char* name;
    int age;
} random_struct_t;

TEST(asm_RDRAND){
    float random1 = 0;
    float random2 = 0;
    ASM_RAND(random1);
    //asm(
    //    "RDRAND %%EAX\r\n"
    //    "CLC" 
    //    : "=a"(random1)
    //);
    printf("%f \n", random1);
    ASM_RAND(random2);
    //asm(
    //   "RDRAND %%EAX\r\n"
    //    : "=a"(random2)
    //);
    printf("%f \n", random2);
    ASSERT_THAT(random1 != random2);
}

TEST(asm_CPUID){
    char* cpu_id = NULL;
    ASM_GET_CPU_NAME(cpu_id);

    ASSERT_THAT(strcmp(cpu_id, "GenuineIntel"));
}

TEST(init_dict){
    dictionary_t* dict = new_dictionary(10);
    ASSERT_THAT(dict != NULL);
    ASSERT_THAT(dict->entries != NULL);
    ASSERT_THAT(dict->hash_size == 10);
    free(dict);
}

TEST(new_entry){
    key_value_t* entry = new_key_value((void *)"some_key", (void *)1938, sizeof("some_key"));
    ASSERT_THAT((char*)entry->key == "some_key");
    ASSERT_THAT((int)entry->value == 1938);
    ASSERT_THAT(entry->next == NULL);
    free(entry);
}

TEST(add_entry){
    dictionary_t* dict = new_dictionary(10);
    key_value_t* entry = new_key_value((void *)"some_key", (void *)1938, sizeof("some_key"));
    register_key_value(dict, entry);
    unsigned int hash = DJBHash((void *)"some_key", sizeof("some_key"));
    hash %= dict->hash_size;
    ASSERT_THAT(dict->entries != NULL);
    ASSERT_THAT(dict->size == 1);
    ASSERT_THAT((char*)dict->entries[hash]->key == "some_key"); 
    ASSERT_THAT((int)dict->entries[hash]->value == 1938); 
    free(dict);
    free(entry);
}

TEST(add_multiple_entries){
    dictionary_t* dict = new_dictionary(10);
    key_value_t* entry1 = new_key_value((void *)"some_key", (void *)1938, sizeof("some_key"));
    register_key_value(dict, entry1);
    key_value_t* entry2 = new_key_value((void *)"some_other_key", (void *)748, sizeof("some_other_key"));
    register_key_value(dict, entry2);

    unsigned int hash0 = DJBHash((void *)"some_key", sizeof("some_key"));
    hash0 %= dict->hash_size;

    unsigned int hash1 = DJBHash((void *)"some_other_key", sizeof("some_other_key"));
    hash1 %= dict->hash_size;

    ASSERT_THAT(dict->entries != NULL);
    ASSERT_THAT(dict->size == 2);
    ASSERT_THAT((char*)dict->entries[hash0]->key == "some_key"); //knowing that hash will return 2
    ASSERT_THAT((int)dict->entries[hash1]->value == 748); //knowing that hash will return 5
    free(dict);
    free(entry1);
    free(entry2);
}

TEST(get_value){
    dictionary_t* dict = new_dictionary(10);
    key_value_t* entry = new_key_value((void *)"some_key", (void *)1938, sizeof("some_key"));
    register_key_value(dict, entry);
    void* value = get_value(dict, (void*)"some_key", sizeof("some_key"));
    ASSERT_THAT((int)value == 1938);
    free(dict);
    free(entry);
}

TEST(get_multiple_value){
    dictionary_t* dict = new_dictionary(10);
    key_value_t* entry1 = new_key_value((void *)"some_key", (void *)1938, sizeof("some_key"));
    register_key_value(dict, entry1);
    void* some_key_value = get_value(dict, (void*)"some_key", sizeof("some_key"));
    key_value_t* entry2 = new_key_value((void *)"some_other_key", (void *)4664, sizeof("some_other_key"));
    register_key_value(dict, entry2);
    void* some_other_key_value = get_value(dict, (void*)"some_other_key", sizeof("some_other_key"));
    ASSERT_THAT((int)some_key_value == 1938);
    ASSERT_THAT((int)some_other_key_value == 4664);
    free(dict);
    free(entry1);
    free(entry2);
}

TEST(get_key_value){
    dictionary_t* dict = new_dictionary(10);
    key_value_t* entry = new_key_value((void *)"some_key", (void *)1938, sizeof("some_key"));
    register_key_value(dict, entry);
    key_value_t* key_value = get_key_value(dict, (void*)"some_key", sizeof("some_key"));
    ASSERT_THAT((char *)key_value->key == "some_key");
    ASSERT_THAT((int)key_value->value == 1938);
    ASSERT_THAT(key_value->next == NULL);
    free(dict);
    free(entry);
}

TEST(replace_value){
    dictionary_t* dict = new_dictionary(10);
    key_value_t* entry = new_key_value((void *)"some_key", (void *)1938, sizeof("some_key"));
    register_key_value(dict, entry);
    key_value_t* entry2 = new_key_value((void *)"some_key", (void *)1111, sizeof("some_key"));
    register_key_value(dict, entry2);
    void* value = get_value(dict, (void*)"some_key", sizeof("some_key"));
    ASSERT_THAT((int)value == 1111);
    free(dict);
    free(entry);
    free(entry2);
}

TEST(remove_key_value){
    dictionary_t* dict = new_dictionary(10);
    key_value_t* entry = new_key_value((void *)"some_key", (void *)1938, sizeof("some_key"));
    register_key_value(dict, entry);
    remove_key_value(dict, (void *)"some_key", sizeof("some_key"));
    void* value = get_value(dict, (void*)"some_key", sizeof("some_key"));
    ASSERT_THAT((int)value == 0);
    free(dict);
}

TEST(remove_key_value_on_collision){
    dictionary_t* dict = new_dictionary(1);
    key_value_t* entry1 = new_key_value((void *)"firstVal", (void *)1, sizeof("firstVal"));
    register_key_value(dict, entry1);
    key_value_t* entry2 = new_key_value((void *)"secondVal", (void *)2, sizeof("secondVal"));
    register_key_value(dict, entry2);
    key_value_t* entry3 = new_key_value((void *)"thirdVal", (void *)3, sizeof("thirdVal"));
    register_key_value(dict, entry3);
    remove_key_value(dict, (void *)"secondVal", sizeof("secondVal"));
    void* value = get_value(dict, (void*)"firstVal", sizeof("firstVal"));
    ASSERT_THAT((int)value == 1);
    value = get_value(dict, (void*)"secondVal", sizeof("secondVal"));
    ASSERT_THAT((int)value == 0);
    value = get_value(dict, (void*)"thirdVal", sizeof("thirdVal"));
    ASSERT_THAT((int)value == 3);

    remove_key_value(dict, (void *)"thirdVal", sizeof("thirdVal"));
    value = get_value(dict, (void*)"thirdVal", sizeof("thirdVal"));
    ASSERT_THAT((int)value == 0);
    free(dict);
    free(entry1);
}

TEST(remove_head_key_value_on_collision){
    dictionary_t* dict = new_dictionary(1);
    key_value_t* entry1 = new_key_value((void *)"firstVal", (void *)1, sizeof("firstVal"));
    register_key_value(dict, entry1);
    key_value_t* entry2 = new_key_value((void *)"secondVal", (void *)2, sizeof("secondVal"));
    register_key_value(dict, entry2);
    key_value_t* entry3 = new_key_value((void *)"thirdVal", (void *)3, sizeof("thirdVal"));
    register_key_value(dict, entry3);
    remove_key_value(dict, (void *)"firstVal", sizeof("firstVal"));
    void* value = get_value(dict, (void*)"firstVal", sizeof("firstVal"));
    ASSERT_THAT((int)value == 0);
    value = get_value(dict, (void*)"secondVal", sizeof("secondVal"));
    ASSERT_THAT((int)value == 2);
    value = get_value(dict, (void*)"thirdVal", sizeof("thirdVal"));
    ASSERT_THAT((int)value == 3);
    free(dict);
    free(entry2);
    free(entry3);
}

TEST(get_value_on_collision){
    dictionary_t* dict = new_dictionary(1);
    key_value_t* entry = new_key_value((void *)"some_key", (void *)1938, sizeof("some_key"));
    register_key_value(dict, entry);
    key_value_t* entry2 = new_key_value((void *)"some_other_key", (void *)1111, sizeof("some_other_key"));
    register_key_value(dict, entry2);
    void* value = get_value(dict, (void*)"some_key", sizeof("some_key"));
    ASSERT_THAT((int)value == 1938);
    void* value2 = get_value(dict, (void*)"some_other_key", sizeof("some_other_key"));
    ASSERT_THAT((int)value2 == 1111);
}

TEST(get_value_with_wrong_key){
    dictionary_t* dict = new_dictionary(10);
    key_value_t* entry = new_key_value((void *)"some_key", (void *)1938, sizeof("some_key"));
    register_key_value(dict, entry);
    void* value = get_value(dict, (void*)"some_other_key", sizeof("some_other_key"));
    ASSERT_THAT(value == NULL);
    key_value_t* key_val = get_key_value(dict, (void*)"some_other_key", sizeof("some_other_key"));
    ASSERT_THAT(key_val == NULL);
}

TEST(get_value_with_int_key){
    dictionary_t* dict = new_dictionary(10);
    int key = 19380;
    key_value_t* entry = new_key_value((void *)&key, (void *)"some_key", sizeof(key));
    register_key_value(dict, entry);
    void* value = get_value(dict, (void*)&key, sizeof(key));
    ASSERT_THAT((char*)value == "some_key");
    free(dict);
    free(entry);
}

TEST(get_value_with_struct_key){
    dictionary_t* dict = new_dictionary(10);
    random_struct_t key;
    key.age = 100;
    key.name = "random_name";
    key_value_t* entry = new_key_value((void *)&key, (void *)"some_key", sizeof(key));
    register_key_value(dict, entry);
    void* value = get_value(dict, (void*)&key, sizeof(key));
    ASSERT_THAT((char*)value == "some_key");
    free(dict);
    free(entry);
}

int main(int argc, char **argv)
{
    RUN_TEST(asm_RDRAND);
    RUN_TEST(asm_CPUID);
    RUN_TEST(init_dict);
    RUN_TEST(new_entry);
    RUN_TEST(add_entry);
    RUN_TEST(add_multiple_entries);
    RUN_TEST(get_value);
    RUN_TEST(get_multiple_value);
    RUN_TEST(get_key_value);
    RUN_TEST(replace_value);
    RUN_TEST(remove_key_value);
    RUN_TEST(remove_key_value_on_collision);
    RUN_TEST(remove_head_key_value_on_collision);
    RUN_TEST(get_value_on_collision);
    RUN_TEST(get_value_with_wrong_key);
    RUN_TEST(get_value_with_int_key);
    RUN_TEST(get_value_with_struct_key);
    PRINT_TEST_RESULTS();
    return 0;
}
