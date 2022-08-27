
/**
 * Questa è la definizione della struttura dati synstr
 */
struct dynstr {
	// ...
};

/**
 * Permette la concatenazione di una parte di stringa su una stringa dinamica
 * @param ds	 la stringa dinamica a cui concatenare la stringa
 * @param suffix il suffisso da concatenare alla stringa dinamica
 */
void append(struct dynstr *ds, const char *suffix)
{
	// ...
}

/**
 * Inizializza una struttura dati dynstr
 * @param ds	la stringa dinamica da inizializzare
 * @param len	la lunghezza iniziale del buffer conservato da ds
 */
void dynstr_init(struct dynstr *ds, size_t len)
{
	// ...
}

/**
 * Stampa a schermo una stringa dinamica
 * @param ds	la stringa dinamica da stampare a schermo
 */
void print(struct dynstr *ds)
{
	// ...
}

int main(void)
{
	struct dynstr ds;
	dynstr_init(&ds, 8);

	append(&ds, "Hello ");
	append(&ds, "world, ");
	append(&ds, "this ");
	append(&ds, "is ");
	append(&ds, "my ");
	append(&ds, "program");

	print(&ds); // Expected print: "Hello world, this is my program"

	return 0;
}
