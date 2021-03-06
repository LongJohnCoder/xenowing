#include "../obj_dir/Valu.h"

#include <cinttypes>
#include <iostream>
using namespace std;

#ifdef _WIN32
#include <Windows.h>
#else
#include <dlfcn.h>
#endif

typedef struct
{
    uint32_t (*get_op)();
    void (*set_op)(uint32_t value);

    uint32_t (*get_lhs)();
    void (*set_lhs)(uint32_t value);
    uint32_t (*get_rhs)();
    void (*set_rhs)(uint32_t value);

    uint32_t (*get_res)();
    void (*set_res)(uint32_t value);

    void (*eval)();
    void (*final)();
} Env;

static Valu *top;

uint32_t get_op()
{
    return top->op;
}

void set_op(uint32_t value)
{
    top->op = value;
}

uint32_t get_lhs()
{
    return top->lhs;
}

void set_lhs(uint32_t value)
{
    top->lhs = value;
}

uint32_t get_rhs()
{
    return top->rhs;
}

void set_rhs(uint32_t value)
{
    top->rhs = value;
}

uint32_t get_res()
{
    return top->res;
}

void set_res(uint32_t value)
{
    top->res = value;
}

void eval()
{
    top->eval();
}

void final()
{
    top->final();
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        cout << "Invalid number of arguments" << endl;
        exit(1);
    }

    auto libName = argv[1];
    auto lib =
#ifdef _WIN32
        LoadLibrary(libName);
#else
        dlopen(libName, RTLD_LAZY);
#endif
    if (!lib)
    {
        cout << "Failed to load library: " << libName << endl;
        exit(1);
    }
    auto run = (int32_t (*)(Env *))
#ifdef _WIN32
        GetProcAddress
#else
        dlsym
#endif
        (lib, "run");
    if (!run)
    {
        cout << "Failed to get run proc address" << endl;
        exit(1);
    }

    top = new Valu();

    Env env =
    {
        .get_op = get_op,
        .set_op = set_op,

        .get_lhs = get_lhs,
        .set_lhs = set_lhs,
        .get_rhs = get_rhs,
        .set_rhs = set_rhs,

        .get_res = get_res,
        .set_res = set_res,

        .eval = eval,
        .final = final,
    };

    auto ret = run(&env);

    delete top;

    return ret;
}
