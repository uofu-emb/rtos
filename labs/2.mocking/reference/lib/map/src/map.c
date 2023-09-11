void map(int *list, int length, int (*func)(int))
{
    for (int i = 0; i < length; i++)
        *(list+i) = func(*(list+i));
}
