#include <nm.h>

static int compare_symbols64(t_symbol* symbol1, t_symbol* symbol2, int flags);
static int compare_symbols32(t_symbol* symbol1, t_symbol* symbol2, int flags);


t_symbol* insert_sorted(t_symbol* symbols, t_symbol* new_symbol, int flags,
                        int (*compare_func)(t_symbol*, t_symbol*, int)) {
    t_symbol* tmp = symbols;
    t_symbol* prev = NULL;
    while (tmp && compare_func(new_symbol, tmp, flags) > 0) {
        prev = tmp;
        tmp = tmp->next;
    }
    if (prev == NULL) {
        new_symbol->next = symbols;
        symbols = new_symbol;
    } else {
        new_symbol->next = prev->next;
        prev->next = new_symbol;
    }
    return symbols;
}


t_symbol* sort_symbols(t_symbol* symbols, int flags,
                       int (*compare_func)(t_symbol*, t_symbol*, int)) {
    t_symbol* sorted = NULL;
    t_symbol* next = NULL;
    
    while (symbols) {
        next = symbols->next;
        symbols->next = NULL;
        sorted = insert_sorted(sorted, symbols, flags, compare_func);
        symbols = next;
    }
    return sorted;
}


int compare_symbols64(t_symbol* symbol1, t_symbol* symbol2, int flags) {
    int str_cmp = ft_strcmp(symbol1->name, symbol2->name);
    if (flags & FLAG_R) str_cmp = -str_cmp;

    
    if (str_cmp == 0) {
        if ((flags & FLAG_R) && symbol1->sym->st_value > symbol2->sym->st_value) return 1;
        if (!(flags & FLAG_R) && symbol1->sym->st_value < symbol2->sym->st_value) return 1;
        return -1;
    }
    return str_cmp;
}


int compare_symbols32(t_symbol* symbol1, t_symbol* symbol2, int flags) {
    int str_cmp = ft_strcmp(symbol1->name, symbol2->name);
    if (flags & FLAG_R) str_cmp = -str_cmp;

    
    if (str_cmp == 0) {
        if ((flags & FLAG_R) && symbol1->sym32->st_value > symbol2->sym32->st_value) return 1;
        if (!(flags & FLAG_R) && symbol1->sym32->st_value < symbol2->sym32->st_value) return 1;
        return -1;
    }
    return str_cmp;
}


t_symbol* _sort64(t_symbol* symbols, int flags) {
    return sort_symbols(symbols, flags, compare_symbols64);
}

t_symbol* _sort32(t_symbol* symbols, int flags){
    return sort_symbols(symbols, flags, compare_symbols32);
}
