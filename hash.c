unsigned int DJBHash(void* key, unsigned int len){
    unsigned int hash = 5381;
    unsigned int i = 0;
    char* ptr = key;
    for(i = 0; i < len; i++)
    {   
        hash += hash << 5;
        hash += *ptr;
        ptr++;
    }   
    return hash;
}
