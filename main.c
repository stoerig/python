#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#if defined(_MSDOS) || defined(_WIN32)
#include <windows.h>
#endif
#include "sqlext.h"

const char *menu_string = "[s] Search for an employee by name\n[i] Search for an employee by ID\n[n] Add a new employee\n[d] Delete an employee\n[m] Modify the salary of an employee\n[q] To Quit\n";
int main(void) {
	SQLHENV eh;
	SQLHDBC ch;
	SQLHSTMT sh;
	SQLCHAR wanted_name[20 + 1];
	SQLINTEGER wanted_name_size;
	int n;
	SQLINTEGER wanted_number;
	SQLINTEGER wanted_number_size;

	/* ODBC 2.x uses SQLAllocEnv, SQLAllocConnect and SQLAllocStmt.
	* ODBC 3.x uses SQLAllocHandle, with different arguments, for all of those.
	* First: Allocate environment handle.
	*/
	if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &eh) != SQL_SUCCESS) {
		fprintf(stderr, "Kunde inte allokera en ODBC-omgivning.\n");
		return EXIT_FAILURE;
	}

	/* Set the ODBC version attribute,
	* so ODBC knows which ODBC version this application was written for.
	*/
	if (SQLSetEnvAttr(eh,
		SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3,
		SQL_IS_INTEGER) != SQL_SUCCESS) {
		fprintf(stderr, "Kunde inte sätta ODBC-versionen till SQL_OV_ODBC3.\n");
		return EXIT_FAILURE;
	}

	/* Allocate connection handle */
	if (SQLAllocHandle(SQL_HANDLE_DBC, eh, &ch) != SQL_SUCCESS) {
		fprintf(stderr, "Kunde inte allokera ett anslutningsobjekt.\n");
		return EXIT_FAILURE;
	}

	/* Connect: data source, user name, password */
	if (SQLConnect(ch, (SQLCHAR*)"dbkcopy", SQL_NTS,
		(SQLCHAR*)"dbtek178", SQL_NTS,
		(SQLCHAR*)"besvikelse", SQL_NTS) != SQL_SUCCESS) {
		fprintf(stderr, "Kunde inte ansluta till datakällan 'demobasen'.\n");
		getchar();
		return EXIT_FAILURE;
	}

	/* Allocate statement handle */
	if (SQLAllocHandle(SQL_HANDLE_STMT, ch, &sh) != SQL_SUCCESS) {
		fprintf(stderr, "Kunde inte allokera ett statement handle.\n");
		return EXIT_FAILURE;
	}

	if (SQLExecDirect(sh, (SQLCHAR*)"select count(*) from personal", SQL_NTS)
		!= SQL_SUCCESS) {
		fprintf(stderr, "Kunde inte köra frågan.\n");
		return EXIT_FAILURE;
	}

	printf("Personer:\n");
	while (SQLFetch(sh) == SQL_SUCCESS) {
		SQLINTEGER number;
		SQLCHAR name[20 + 1];
		SQLINTEGER number_size, name_size;

		SQLGetData(sh, 1, SQL_C_SLONG, &number, sizeof number, &number_size);
		//SQLGetData(sh, 2, SQL_C_CHAR, name, sizeof name, &name_size);

		printf("Count %d, av personer\n", (int)number);
	}

	/* You have to close the cursor before re-using the statement handle! */
	SQLCloseCursor(sh);

	/*printf("Ange namnet på en av personerna: ");
	fgets(wanted_name, sizeof wanted_name, stdin);
	if (wanted_name[strlen(wanted_name) - 1] == '\n')
	wanted_name[strlen(wanted_name) - 1] = '\0';
	printf("Du har valt namnet '%s'.\n", wanted_name);*/

	//printf("Ange numret på en av personerna: ");
	/*if (scanf_s("%d", &n) != 1) {
	fprintf(stderr, "Det där var inte ett korrekt formaterat heltal.\n");
	return EXIT_FAILURE;
	}*/
	/* Skip the rest of the line, after the integer */
	//while (getchar() != '\n');
	/* We want to be sure that we use the correct data types,
	both for sscanf (int) and SQLBindParameter (SQL_C_SLONG). */
	/*wanted_number = n;
	printf("Du har valt numret %d.\n", (int)wanted_number);*/
	/*SQLBindParameter(sh, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR,
	20, 0, wanted_name, sizeof wanted_name, &wanted_name_size);*/
	/*SQLBindParameter(sh, 2, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,
	0, 0, &wanted_number, sizeof wanted_number, &wanted_number_size);
	*/
	char switch_ch;
	SQLCHAR user_input[30 + 1];
	SQLINTEGER user_input_size;
	printf("%s", menu_string);
	while ((switch_ch = tolower(getchar())) != 'q') {
		fseek(stdin, 0, SEEK_END);
		switch (switch_ch) {
		case 's':
			printf("Enter Name of Employee:\n");
			fgets(user_input, 31, stdin);
			//printf("%s\n", user_input);
			if (user_input[strlen(user_input) - 1] == '\n')
				user_input[strlen(user_input) - 1] = '\0';
			SQLBindParameter(sh, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR,
				20, 0, user_input, sizeof user_input, &user_input_size);
			user_input_size = SQL_NTS;
			if (SQLExecDirect(sh, (SQLCHAR*)"SELECT id, personalnummer, namn, gatuadress, postnummer, postadress, telefon FROM personal"
				" WHERE namn = ?", SQL_NTS) != SQL_SUCCESS) {
				fprintf(stderr, "SQL-frågan misslyckades.\n");
				getchar();
				return EXIT_FAILURE;
			}

			printf("De sökta personerna:\n");
			while (SQLFetch(sh) == SQL_SUCCESS) {
				SQLINTEGER id, personalnummer;
				SQLCHAR name[30 + 1], gatuadress[20 + 1], postnummer[10 + 1], postadress[20 + 1], telefon[20 + 1];
				SQLINTEGER id_size, personalnummer_size, name_size, gatuadress_size, postnummer_size, postadress_size, telefon_size;

				//SQLGetData(sh, 1, SQL_C_SLONG, &number, sizeof number, &number_size);
				SQLGetData(sh, 1, SQL_C_LONG, &id, sizeof id, &id_size);
				SQLGetData(sh, 2, SQL_C_LONG, &personalnummer, sizeof personalnummer, &personalnummer_size);
				SQLGetData(sh, 3, SQL_C_CHAR, name, sizeof name, &name_size);
				SQLGetData(sh, 4, SQL_C_CHAR, gatuadress, sizeof gatuadress, &gatuadress_size);
				SQLGetData(sh, 5, SQL_C_CHAR, postnummer, sizeof postnummer, &postnummer_size);
				SQLGetData(sh, 6, SQL_C_CHAR, postadress, sizeof postadress, &postadress_size);
				SQLGetData(sh, 7, SQL_C_CHAR, telefon, sizeof telefon, &telefon_size);

				printf("************\nId: %d\nPersonalnummer: %d\nNamn: %s\nGatuadress: %s\nPostnummer: %s\nPostadress: %s\nTelefon: %s\n************\n",
					id, personalnummer, name, gatuadress, postnummer, postadress, telefon);
			}
			SQLCloseCursor(sh);
			if (SQLAllocHandle(SQL_HANDLE_STMT, ch, &sh) != SQL_SUCCESS) {
				fprintf(stderr, "Kunde inte allokera ett statement handle.\n");
				return EXIT_FAILURE;
			}
			fseek(stdin, 0, SEEK_END);
			user_input[0] = '\0';
			user_input_size = 0;
			printf("%s", menu_string);
			break;
		case 'i':
			printf("Enter ID of Employee:\n");
			fgets(user_input, 21, stdin);
			//printf("%s\n", user_input);
			if (user_input[strlen(user_input) - 1] == '\n')
				user_input[strlen(user_input) - 1] = '\0';
			SQLBindParameter(sh, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR,
				20, 0, user_input, sizeof user_input, &user_input_size);
			user_input_size = SQL_NTS;

			if (SQLExecDirect(sh, (SQLCHAR*)"SELECT id, personalnummer, namn, gatuadress, postnummer, postadress, telefon FROM personal"
				" WHERE id = ?", SQL_NTS) != SQL_SUCCESS) {
				fprintf(stderr, "SQL-frågan misslyckades.\n");
				getchar();
				return EXIT_FAILURE;
			}

			printf("De sökta personerna:\n");

			while (SQLFetch(sh) == SQL_SUCCESS) {
				SQLINTEGER id, personalnummer;
				SQLCHAR name[30 + 1], gatuadress[20 + 1], postnummer[10 + 1], postadress[20 + 1], telefon[20 + 1];
				SQLINTEGER id_size, personalnummer_size, name_size, gatuadress_size, postnummer_size, postadress_size, telefon_size;

				SQLGetData(sh, 1, SQL_C_LONG, &id, sizeof id, &id_size);
				SQLGetData(sh, 2, SQL_C_LONG, &personalnummer, sizeof personalnummer, &personalnummer_size);
				SQLGetData(sh, 3, SQL_C_CHAR, name, sizeof name, &name_size);
				SQLGetData(sh, 4, SQL_C_CHAR, gatuadress, sizeof gatuadress, &gatuadress_size);
				SQLGetData(sh, 5, SQL_C_CHAR, postnummer, sizeof postnummer, &postnummer_size);
				SQLGetData(sh, 6, SQL_C_CHAR, postadress, sizeof postadress, &postadress_size);
				SQLGetData(sh, 7, SQL_C_CHAR, telefon, sizeof telefon, &telefon_size);

				printf("************\nId: %d\nPersonalnummer: %d\nNamn: %s\nGatuadress: %s\nPostnummer: %s\nPostadress: %s\nTelefon: %s\n************\n",
					id, personalnummer, name, gatuadress, postnummer, postadress, telefon);
			}
			SQLCloseCursor(sh);
			if (SQLAllocHandle(SQL_HANDLE_STMT, ch, &sh) != SQL_SUCCESS) {
				fprintf(stderr, "Kunde inte allokera ett statement handle.\n");
				return EXIT_FAILURE;
			}
			fseek(stdin, 0, SEEK_END);
			user_input[0] = '\0';
			user_input_size = 0;
			printf("%s", menu_string);
			printf("Searching by index..\n");
			break;
		case 'n':
			printf("Enter information of employee to be added..\nEnter Personalnummer of Employee:\n");
			fgets(user_input, 21, stdin);
			//printf("%s\n", user_input);
			if (user_input[strlen(user_input) - 1] == '\n')
				user_input[strlen(user_input) - 1] = '\0';
			SQLBindParameter(sh, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR,
				20, 0, user_input, sizeof user_input, &user_input_size);
			user_input_size = SQL_NTS;

			if (SQLExecDirect(sh, (SQLCHAR*)"SELECT id, personalnummer, namn, gatuadress, postnummer, postadress, telefon FROM personal"
				" WHERE id = ?", SQL_NTS) != SQL_SUCCESS) {
				fprintf(stderr, "SQL-frågan misslyckades.\n");
				getchar();
				return EXIT_FAILURE;
			}

			printf("De sökta personerna:\n");
			while (SQLFetch(sh) == SQL_SUCCESS) {
				SQLINTEGER id, personalnummer;
				SQLCHAR name[30 + 1], gatuadress[20 + 1], postnummer[10 + 1], postadress[20 + 1], telefon[20 + 1];
				SQLINTEGER id_size, personalnummer_size, name_size, gatuadress_size, postnummer_size, postadress_size, telefon_size;

				SQLGetData(sh, 1, SQL_C_LONG, &id, sizeof id, &id_size);
				SQLGetData(sh, 2, SQL_C_LONG, &personalnummer, sizeof personalnummer, &personalnummer_size);
				SQLGetData(sh, 3, SQL_C_CHAR, name, sizeof name, &name_size);
				SQLGetData(sh, 4, SQL_C_CHAR, gatuadress, sizeof gatuadress, &gatuadress_size);
				SQLGetData(sh, 5, SQL_C_CHAR, postnummer, sizeof postnummer, &postnummer_size);
				SQLGetData(sh, 6, SQL_C_CHAR, postadress, sizeof postadress, &postadress_size);
				SQLGetData(sh, 7, SQL_C_CHAR, telefon, sizeof telefon, &telefon_size);

				printf("************\nId: %d\nPersonalnummer: %d\nNamn: %s\nGatuadress: %s\nPostnummer: %s\nPostadress: %s\nTelefon: %s\n************\n",
					id, personalnummer, name, gatuadress, postnummer, postadress, telefon);
			}
			SQLCloseCursor(sh);
			if (SQLAllocHandle(SQL_HANDLE_STMT, ch, &sh) != SQL_SUCCESS) {
				fprintf(stderr, "Kunde inte allokera ett statement handle.\n");
				return EXIT_FAILURE;
			}
			fseek(stdin, 0, SEEK_END);
			user_input[0] = '\0';
			user_input_size = 0;
			printf("%s", menu_string);
			printf("Searching by index..\n");
			printf("Adding a new employee..\n");
			break;
		case 'd':
			printf("Enter Personalnummer of employee to be deleted..\n");
			if (scanf_s("%d", &n) != 1) {
				fprintf(stderr, "Det där var inte ett korrekt formaterat heltal.\n");
				getchar();
				return EXIT_FAILURE;
			}
			/* Skip the rest of the line, after the integer */
			//while (getchar() != '\n');
			/*We want to be sure that we use the correct data types,
			both for sscanf (int) and SQLBindParameter (SQL_C_SLONG). */
			wanted_number = n;

			SQLBindParameter(sh, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,
				20, 0, &wanted_number, sizeof wanted_number, &wanted_number_size);

			wanted_number_size = sizeof wanted_number;

			printf("Wanted number: %d\n", wanted_number);
			getchar();

			if (SQLExecDirect(sh, (SQLCHAR*)"DELETE FROM personal"
				" WHERE personalnummer = ?", SQL_NTS) != SQL_SUCCESS) {
				fprintf(stderr, "SQL-frågan misslyckades.\n");
				getchar();
				return EXIT_FAILURE;
			}

			printf("Anstalld med perosnalnummer: %s\n", user_input);

			while (SQLFetch(sh) == SQL_SUCCESS) {
				SQLINTEGER id, personalnummer;
				SQLCHAR name[30 + 1], gatuadress[20 + 1], postnummer[10 + 1], postadress[20 + 1], telefon[20 + 1];
				SQLINTEGER id_size, personalnummer_size, name_size, gatuadress_size, postnummer_size, postadress_size, telefon_size;

				//SQLGetData(sh, 1, SQL_C_SLONG, &number, sizeof number, &number_size);
				SQLGetData(sh, 1, SQL_C_LONG, &id, sizeof id, &id_size);
				SQLGetData(sh, 2, SQL_C_LONG, &personalnummer, sizeof personalnummer, &personalnummer_size);
				SQLGetData(sh, 3, SQL_C_CHAR, name, sizeof name, &name_size);
				SQLGetData(sh, 4, SQL_C_CHAR, gatuadress, sizeof gatuadress, &gatuadress_size);
				SQLGetData(sh, 5, SQL_C_CHAR, postnummer, sizeof postnummer, &postnummer_size);
				SQLGetData(sh, 6, SQL_C_CHAR, postadress, sizeof postadress, &postadress_size);
				SQLGetData(sh, 7, SQL_C_CHAR, telefon, sizeof telefon, &telefon_size);

				printf("************\nId: %d\nPersonalnummer: %d\nNamn: %s\nGatuadress: %s\nPostnummer: %s\nPostadress: %s\nTelefon: %s\n************\n",
					id, personalnummer, name, gatuadress, postnummer, postadress, telefon);
			}
			SQLCloseCursor(sh);
			if (SQLAllocHandle(SQL_HANDLE_STMT, ch, &sh) != SQL_SUCCESS) {
				fprintf(stderr, "Kunde inte allokera ett statement handle.\n");
				return EXIT_FAILURE;
			}
			fseek(stdin, 0, SEEK_END);
			user_input[0] = '\0';
			user_input_size = 0;
			printf("%s", menu_string);
			printf("Searching by index..\n");
			printf("Adding a new employee..\n");
			printf("Deleting a employee..\n");
			break;
		case 'm':
			printf("Modifying an employee..\n");
			break;
		case 'q':
			printf("Quitting..\n");
			break;
		default:
			break;
		}
	}

	SQLFreeHandle(SQL_HANDLE_STMT, sh);
	SQLDisconnect(ch);
	SQLFreeHandle(SQL_HANDLE_DBC, ch);
	SQLFreeHandle(SQL_HANDLE_ENV, eh);

	return EXIT_SUCCESS;
}