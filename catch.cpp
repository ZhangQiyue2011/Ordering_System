#include <queue>
#define maxorders 100000000

struct Order
{
    bool cookdone;
    //pass
};

struct Program
{
    bool have;
    Order ord;
    //pass
};


Order que[maxorders];
size_t start = 0, end = 1;

void push_in_que(Order &ord)
{
    //pass
}

void pop_in_que()
{
    //pass
}

Program *programs;
size_t program_cnt;


void intoque()
{
    for (size_t i = 0; i < program_cnt; ++i)
    {
        if (programs[i].have)
        {
            push_in_que(programs[i].ord);
        }
    }
}

void pick_up_meals()
{
    for (size_t i = start; i != end; i = (i + 1) % maxorders)
    {
        if (que[i].cookdone)
        {
            //pass Enable customers to pick up their meals.
            pop_in_que();
        }
    }
}

int main(int argc, char const *argv[])
{
    programs = new Program[10]{0, {0}};
    program_cnt = 10;
    while (true)
    {
        intoque();
        pick_up_meals();
        //After completing the order for food, set the coolone variable in the order to true here.
    }
    return 0;
}
