#include <nm.h>

static int compare_symbols64(t_symbol* symbol1, t_symbol* symbol2, int flags);
static int compare_symbols32(t_symbol* symbol1, t_symbol* symbol2, int flags);

int ft_alnum_strcmp(const char *a, const char *b){
	// Same as strcmp but taking only into account alpahnumeric characters
	// and ignoring case. 

	while (*a && *b){
		if (ft_isalnum(*a) && ft_isalnum(*b)){
			if (ft_tolower(*a) != ft_tolower(*b))
				return ft_tolower(*a) - ft_tolower(*b);
			++a;
			++b;
		}
		else if (ft_strchr("_@.", *a))
			++a;
		else if (ft_strchr("_@.", *b))
			++b;
		else if (ft_isalnum(*a))
			return 1;
		else if (ft_isalnum(*b))
			return -1;
		else {
			++a;
			++b;
		}
	}

	if (*a)
		return 1;
	if (*b)
		return -1;
	return 0;
}

t_symbol* insert_sorted(t_symbol* symbols,
						t_symbol* new_symbol,
						int flags,
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

static
t_symbol* sort_symbols(t_symbol* symbols,
					   int flags,
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

static 
const char *get_first_alphanum_char(const char *str){
	while (!ft_isalnum(*str))
		++str;
	return str;
}

int compare_symbols64(t_symbol* symbol1, t_symbol* symbol2, int flags) {
	unsigned char type1 = ELF64_ST_TYPE(symbol1->sym->st_info);
	unsigned char type2 = ELF64_ST_TYPE(symbol2->sym->st_info);
	int str_cmp = ft_alnum_strcmp(get_first_alphanum_char(symbol1->name),
								  get_first_alphanum_char(symbol2->name));
	if (str_cmp == 0)
		str_cmp = ft_strcmp(symbol1->name, symbol2->name);
	if (flags & FLAG_R)
		str_cmp = -str_cmp;

	if (str_cmp == 0) {
		// if (type1 == type2){
		// 	printf("st_value: %lx\n", symbol1->sym->st_value);
		// 	printf("st_value: %lx\n", symbol2->sym->st_value);
		// }
		// 	// compare by st_value
		// 	if (symbol1->sym->st_value != symbol2->sym->st_value) {
		// 		return ((flags & FLAG_R) ^
		// 				(symbol1->sym->st_value < symbol2->sym->st_value))
		// 				   ? 1
		// 				   : -1;
		// 	}
		// }
		if (type1 != type2) {
			return (type1 > type2) ? 1 : -1;
		}
		// if (symbol1->sym->st_value != symbol2->sym->st_value) {
		// 	if (symbol1->type > symbol2->type)
		// 		return -1;

		// 	return ((flags & FLAG_R) ^
		// 			(symbol1->sym->st_value <= symbol2->sym->st_value))
		// 			   ? -1
		// 			   : 1;
		// } else {

		// 	return ((flags & FLAG_R) ^
		// 			(symbol1->sym->st_value < symbol2->sym->st_value))
		// 			   ? 1
		// 			   : -1;
		// }
	}

	return str_cmp;
}

int compare_symbols32(t_symbol* symbol1, t_symbol* symbol2, int flags) {
	int str_cmp = ft_strcmp(symbol1->name, symbol2->name);
	if (flags & FLAG_R)
		str_cmp = -str_cmp;

	if (str_cmp == 0) {
		unsigned char type1 = ELF32_ST_TYPE(symbol1->sym32->st_info);
		unsigned char type2 = ELF32_ST_TYPE(symbol2->sym32->st_info);

		if (type1 != type2) {
			return (type1 > type2) ? 1 : -1;
		} 
		if (ft_tolower(symbol1->type) == ft_tolower(symbol2->type)) {
			return (symbol1->type < symbol2->type)
					   ? 1
					   : -1;
		}

		if (symbol1->sym32->st_value != symbol2->sym32->st_value) {
			return ((flags & FLAG_R) ^
					(symbol1->sym32->st_value > symbol2->sym32->st_value))
					   ? 1
					   : -1;
		}

		
	}

	return str_cmp;
}

t_symbol* _sort64(t_symbol* symbols, int flags) {
	return (sort_symbols(symbols, flags, compare_symbols64));
}

t_symbol* _sort32(t_symbol* symbols, int flags) {
	return (sort_symbols(symbols, flags, compare_symbols32));
}
