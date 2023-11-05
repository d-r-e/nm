#include <nm.h>

t_symbol* _sort64(t_symbol* symbols, int flags) {
	t_symbol* sorted = NULL;
	t_symbol* tmp = NULL;
	t_symbol* prev = NULL;
	t_symbol* next = NULL;
	int str_cmp;

	while (symbols) {
		next = symbols->next;
		if (sorted == NULL) {
			sorted = symbols;
			sorted->next = NULL;
		} else {
			tmp = sorted;
			prev = NULL;
			while (tmp) {
				if (flags & FLAG_R)
					str_cmp = -ft_strcmp(symbols->name, tmp->name);
				else
					str_cmp = ft_strcmp(symbols->name, tmp->name);
				if (str_cmp < 0 || str_cmp == 0) {
					if (str_cmp == 0 && (flags & FLAG_R)) {
						if (symbols->sym->st_value > tmp->sym->st_value)
							break;
					} else if (str_cmp == 0 && !(flags & FLAG_R)) {
						if (symbols->sym->st_value < tmp->sym->st_value) {
							break;
						}
					} else if (str_cmp < 0) {
						break;
					}
					break;
				}
				prev = tmp;
				tmp = tmp->next;
			}
			if (prev == NULL) {
				symbols->next = sorted;
				sorted = symbols;
			} else {
				symbols->next = prev->next;
				prev->next = symbols;
			}
		}
		symbols = next;
	}
	return sorted;
}

t_symbol *_sort32(t_symbol* symbols, int flags){
    /** igual, pero usa ->sym32 en lugar de ->sym*/
    t_symbol* sorted = NULL;
    t_symbol* tmp = NULL;
    t_symbol* prev = NULL;
    t_symbol* next = NULL;

    int str_cmp;
    
    while (symbols) {
        next = symbols->next;
        if (sorted == NULL) {
            sorted = symbols;
            sorted->next = NULL;
        } else {
            tmp = sorted;
            prev = NULL;
            while (tmp) {
                if (flags & FLAG_R)
                    str_cmp = -ft_strcmp(symbols->name, tmp->name);
                else
                    str_cmp = ft_strcmp(symbols->name, tmp->name);
                if (str_cmp < 0 || str_cmp == 0) {
                    if (str_cmp == 0 && (flags & FLAG_R)) {
                        if (symbols->sym32->st_value > tmp->sym32->st_value)
                            break;
                    } else if (str_cmp == 0 && !(flags & FLAG_R)) {
                        if (symbols->sym32->st_value < tmp->sym32->st_value) {
                            break;
                        }
                    } else if (str_cmp < 0) {
                        break;
                    }
                    break;
                }
                prev = tmp;
                tmp = tmp->next;
            }
            if (prev == NULL) {
                symbols->next = sorted;
                sorted = symbols;
            } else {
                symbols->next = prev->next;
                prev->next = symbols;
            }
        }
        symbols = next;
    }

    return sorted;
}