

class DatabaseInterface
{
    DatabaseInterface(char* name);

    select(uint64_t row, char* col);

    select(uint64_t row, char*[] cols);

}