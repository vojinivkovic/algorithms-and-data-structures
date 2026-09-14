#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <cstdlib>

using namespace std;



int read_number_of_nodes(string line) {

	return stoi(line);
}

int read_number_of_edges(string line) {

	return stoi(line);
}

string* read_words_from_file(string line, int number_of_nodes) {

	string* words = new string[number_of_nodes];
	int pos = 0;
	int j = 0;

	for (int i = 0; i < line.length(); i++) {
		if (line[i] == ' ') {
			words[j] = line.substr(pos, i - pos);
			j++;
			pos = i + 1;
		}
	}

	words[j] = line.substr(pos, line.length() - pos);
	return words;
}

string* reallocate_memory(string* nodes, int number_of_nodes, int if_adding) {

	string* new_nodes = new string[number_of_nodes];
	if (if_adding == 1) {
		for (int i = 0; i < number_of_nodes - 1; i++) {
			new_nodes[i] = nodes[i];
		}
	}
	else {
		for (int i = 0; i < number_of_nodes; i++) {
			new_nodes[i] = nodes[i];
		}
	}

	delete[] nodes;
	return new_nodes;

}

int find_index_of_word_in_array(string* words, string word, int number_of_nodes) {

	for (int i = 0; i < number_of_nodes; i++) {
		if (words[i] == word) {
			return i;
		}
	}
	return -1;
}

void get_words_from_line(string line, string* const words) {

	int pos = 0;
	int j = 0;

	for (int i = 0; i < line.length(); i++) {
		if (line[i] == ' ') {
			words[j] = line.substr(pos, i - pos);
			j++;
			pos = i + 1;
		}
	}

	words[j] = line.substr(pos, line.length() - pos);
}

double** make_a_matrix_of_graph(int number_of_nodes, ifstream& file, string* nodes) {


	double** matrix;
	string line;
	int pos_i = 0;
	int pos_j = 0;
	string words[3];

	matrix = (double**)malloc(number_of_nodes * sizeof(double*));
	for (int i = 0; i < number_of_nodes; i++) {
		matrix[i] = (double*)malloc(number_of_nodes * sizeof(double));
	}

	while (getline(file, line)) {
		get_words_from_line(line, words);
		int m = find_index_of_word_in_array(nodes, words[0], number_of_nodes);
		int n = find_index_of_word_in_array(nodes, words[1], number_of_nodes);
		while (pos_i < m) {
			while (pos_j < number_of_nodes) {
				matrix[pos_i][pos_j] = 0;
				pos_j++;
			}
			pos_i++;
			pos_j = 0;
		}
		for (; pos_j < n; pos_j++) {
			matrix[pos_i][pos_j] = 0;
		}

		matrix[m][n] = stod(words[2]);
		pos_i = m;
		pos_j = n + 1;
	}

	while (pos_i < number_of_nodes) {
		while (pos_j < number_of_nodes) {
			matrix[pos_i][pos_j] = 0;
			pos_j++;
		}
		pos_i++;
		pos_j = 0;
	}

	for (; pos_j < number_of_nodes; pos_j++) {
		matrix[pos_i - 1][pos_j] = 0;
	}

	file.close();
	return matrix;
}

string* add_node_in_graph(string node, int& number_of_nodes, string* nodes, double** matrix_of_graph) {

	number_of_nodes++;
	matrix_of_graph = (double**)realloc(matrix_of_graph, number_of_nodes * sizeof(double*));

	for (int i = 0; i < number_of_nodes - 1; i++) {
		matrix_of_graph[i] = (double*)realloc(matrix_of_graph[i], number_of_nodes * sizeof(double));
	}
	matrix_of_graph[number_of_nodes - 1] = (double*)malloc(number_of_nodes * sizeof(double));

	for (int i = 0; i < number_of_nodes; i++) {
		matrix_of_graph[i][number_of_nodes - 1] = 0;
	}
	for (int j = 0; j < number_of_nodes; j++) {
		matrix_of_graph[number_of_nodes - 1][j] = 0;
	}

	nodes = reallocate_memory(nodes, number_of_nodes, 1);
	nodes[number_of_nodes - 1] = node;

	return nodes;
}

string* remove_node_from_graph(string node, int& number_of_nodes, int& number_of_edges, string* nodes, double** matrix_of_graph) {

	int k;
	k = find_index_of_word_in_array(nodes, node, number_of_nodes);

	if (k == -1) {
		cout << "Cvor koji ste izabrali ne postoji" << "\n";
		return nodes;
	}

	for (int i = k + 1; i < number_of_nodes; i++) {
		nodes[i - 1] = nodes[i];
	}

	for (int j = 0; j < number_of_nodes; j++) {
		if (matrix_of_graph[k][j] != -1) {
			number_of_edges--;
		}
	}
	for (int i = 0; i < number_of_nodes; i++) {
		if (matrix_of_graph[i][k] != -1) {
			number_of_edges--;
		}
	}
	for (int i = k + 1; i < number_of_nodes; i++) {
		matrix_of_graph[i - 1] = matrix_of_graph[i];
	}

	for (int i = 0; i < number_of_nodes - 1; i++) {
		for (int j = k + 1; j < number_of_nodes; j++) {
			matrix_of_graph[i][j - 1] = matrix_of_graph[i][j];
		}
	}

	number_of_nodes--;
	nodes = reallocate_memory(nodes, number_of_nodes, 0);

	matrix_of_graph = (double**)realloc(matrix_of_graph, number_of_nodes * sizeof(double*));
	for (int i = 0; i < number_of_nodes; i++) {
		matrix_of_graph[i] = (double*)realloc(matrix_of_graph[i], number_of_nodes * sizeof(double));
	}

	return nodes;
}

int add_edge_in_graph(string line, int number_of_nodes, int& number_of_edges, string* nodes, double** matrix_of_graph) {

	int i, j;
	string words[3];
	get_words_from_line(line, words);

	i = find_index_of_word_in_array(nodes, words[0], number_of_nodes);
	if (i == -1) {
		cout << "Prvi cvor koji ste uneli ne postoji" << "\n";
		return 0;
	}

	j = find_index_of_word_in_array(nodes, words[1], number_of_nodes);
	if (j == -1) {
		cout << "Drugi cvor koji ste uneli ne postoji" << "\n";
		return 0;
	}

	if (stod(words[2]) >= 1 || stod(words[2]) < 0) {
		cout << "Uneli ste neodgovarajucu tezinu grane" << "\n";
		return 0;
	}

	matrix_of_graph[i][j] = stod(words[2]);
	number_of_edges++;
	return 1;
}

void remove_edge_in_graph(string line, int number_of_nodes, int& number_of_edges, string* nodes, double** matrix_of_graph) {

	int i, j;
	string words[2];
	get_words_from_line(line, words);

	i = find_index_of_word_in_array(nodes, words[0], number_of_nodes);
	if (i == -1) {
		cout << "Prvi cvor koji ste uneli ne postoji" << "\n";
		return;
	}

	j = find_index_of_word_in_array(nodes, words[1], number_of_nodes);
	if (j == -1) {
		cout << "Drugi cvor koji ste uneli ne postoji" << "\n";
		return;
	}

	matrix_of_graph[i][j] = 0;
	number_of_edges--;
}

void represent_graph(string* nodes, double** matrix_of_graph, int number_of_nodes) {

	for (int i = 0; i < number_of_nodes; i++) {
		cout << nodes[i] << "|";
		for (int j = 0; j < number_of_nodes; j++) {
			cout << matrix_of_graph[i][j] << " ";
		}
		cout << "\n";
	}
}

void delete_memory(string* nodes, double** matrix_of_graph, int number_of_nodes, double* path_to_node, string* previous_nodes, string* k_similar_words, string* strong_connected_words) {

	delete[] path_to_node;
	delete[] previous_nodes;
	delete[] k_similar_words;
	delete[] nodes;
	delete[] strong_connected_words;

	for (int i = 0; i < number_of_nodes; i++) {
		free(matrix_of_graph[i]);
	}
	free(matrix_of_graph);

}

int find_word(string word, string* finished_words, int number_of_nodes) {
	for (int i = 0; i < number_of_nodes; i++) {
		if (finished_words[i] == word) {
			return 1;
		}
	}
	return 0;
}

int empty_row(double** matrix_of_graph, int number_of_nodes, int row) {

	for (int j = 0; j < number_of_nodes; j++) {
		if (matrix_of_graph[row][j] != 0) {
			return 0;
		}
	}
	return 1;
}


string find_max_word_in_array(double* path_to_node, string* finished_words, string* nodes, int number_of_nodes, int& number_of_finished_words, double** matrix_of_graph) {

	string word = "";
	double max_weight = -1;
	int index_of_max_word;
	int flag = 0;


	for (int i = 0; i < number_of_nodes; i++) {
		if (path_to_node[i] > max_weight && (find_word(nodes[i], finished_words, number_of_nodes) == 0) && (path_to_node[i] != 10)) {
			flag = 1;
			index_of_max_word = i;
			max_weight = path_to_node[i];
		}
	}

	if (flag) {
		word = nodes[index_of_max_word];
		number_of_finished_words++;
		finished_words[number_of_finished_words - 1] = word;
	}

	if (flag) {
		return word;
	}
	return "";
}

void sort_path(double* path_to_node, int number_of_nodes) {

	for (int i = 0; i < number_of_nodes; i++) {
		for (int j = i + 1; j < number_of_nodes; j++) {
			if (path_to_node[i] < path_to_node[j]) {
				double t = path_to_node[i];
				path_to_node[i] = path_to_node[j];
				path_to_node[j] = t;
			}
		}
	}
}

int check_if_traversed(int* traversed_index, int number_of_traversed_index, int k) {

	for (int i = 0; i < number_of_traversed_index - 1; i++) {
		if (traversed_index[i] == k) {
			return 1;
		}
	}
	return 0;
}

int find_index(double* old_path, double weight, int number_of_nodes, int* traversed_index, int& number_of_traversed_index) {

	for (int i = 0; i < number_of_nodes; i++) {
		if (old_path[i] == weight && !check_if_traversed(traversed_index, number_of_traversed_index, i)) {
			traversed_index[number_of_traversed_index - 1] = i;
			number_of_traversed_index++;
			return i;
		}
	}
}

double return_weight_between_two_nodes(string node1, string node2, double** matrix_of_graph, string* nodes, int number_of_nodes) {

	int i = find_index_of_word_in_array(nodes, node1, number_of_nodes);
	int j = find_index_of_word_in_array(nodes, node2, number_of_nodes);

	return matrix_of_graph[i][j];
}

void find_the_shortest_path(string* previous_nodes, int number_of_nodes, double** matrix_of_graph, string* nodes, string node1, string node2) {

	string* path = new string[number_of_nodes];
	int temp = 0;
	int index = find_index_of_word_in_array(nodes, node2, number_of_nodes);
	string new_node;
	int flag = 1;
	int check_index = find_index_of_word_in_array(nodes, node1, number_of_nodes);

	if (check_index == -1) {
		cout << "Prvi cvor koji ste uneli ne postoji" << "\n";
		return;
	}
	if (index == -1) {
		cout << "Drugi cvor koji ste uneli ne postoji" << "\n";
		return;
	}
	if (previous_nodes[index] == "") {
		cout << "Ne postoji put izmedju dva cvora" << "\n";
		return;
	}
	path[temp] = nodes[index];
	temp++;
	new_node = previous_nodes[index];

	while (true) {

		if (new_node == node1) {
			path[temp] = new_node;
			temp++;
			break;

		}
		path[temp] = new_node;
		temp++;
		index = find_index_of_word_in_array(nodes, new_node, number_of_nodes);
		new_node = previous_nodes[index];
	}

	for (int i = temp - 1; i >= 1; i--) {
		cout << path[i] << "-(" << return_weight_between_two_nodes(path[i], path[i - 1], matrix_of_graph, nodes, number_of_nodes) << ")->";

	}
	cout << node2;
	cout << "\n";
	delete[] path;
}

string* find_k_similar_words(string word, string* nodes, double** matrix_of_graph, int number_of_nodes, int  k, double* path_to_node, string* previous_nodes) {

	int number_of_finished_words = 1;
	int number_of_previous_nodes = number_of_finished_words;
	string* finished_words = new string[number_of_nodes];
	int index_of_word = find_index_of_word_in_array(nodes, word, number_of_nodes);
	string* new_words = new string[k];
	double* old_path = new double[number_of_nodes];
	int pos = 0;
	int flag = 0;
	string previous_node = word;

	if (index_of_word == -1) {
		cout << "Rec koju ste uneli ne postoji" << "\n";
		return new_words;
	}

	if (empty_row(matrix_of_graph, number_of_nodes, index_of_word)) {
		cout << "Zadati cvor nije povezana ni sa jednim cvorom" << "\n";
		delete[] finished_words;
		delete[] old_path;
		return new_words;
	}

	finished_words[number_of_finished_words - 1] = word;
	number_of_finished_words++;


	for (int j = 0; j < number_of_nodes; j++) {
		if (matrix_of_graph[index_of_word][j] == 0) {
			path_to_node[j] = 10;
			previous_nodes[j] = "";
		}
		else {
			path_to_node[j] = matrix_of_graph[index_of_word][j];
			previous_nodes[j] = word;
		}
	}

	while (true) {
		string word = find_max_word_in_array(path_to_node, finished_words, nodes, number_of_nodes, number_of_finished_words, matrix_of_graph);
		if (word == "") {
			break;
		}
		index_of_word = find_index_of_word_in_array(nodes, word, number_of_nodes);
		for (int j = 0; j < number_of_nodes; j++) {
			if (matrix_of_graph[index_of_word][j] != 0 and find_word(nodes[j], finished_words, number_of_nodes) == 0) {
				if (previous_nodes[j] == "") {
					previous_nodes[j] = word;
				}
				if (path_to_node[j] != 10) {
					path_to_node[j] = max(path_to_node[j], path_to_node[index_of_word] * matrix_of_graph[index_of_word][j]);
				}
				else {
					path_to_node[j] = path_to_node[index_of_word] * matrix_of_graph[index_of_word][j];
				}
			}
		}
	}


	for (int i = 0; i < number_of_nodes; i++) {
		old_path[i] = path_to_node[i];
	}

	sort_path(path_to_node, number_of_nodes);

	int* traversed_index = new int[number_of_nodes];
	int number_of_traversed_index = 1;
	int temp = 1;
	for (int i = 0; i < number_of_nodes; i++) {
		if (path_to_node[i] != 10) {
			if (temp <= k) {
				int index = find_index(old_path, path_to_node[i], number_of_nodes, traversed_index, number_of_traversed_index);
				new_words[temp - 1] = nodes[index];
				temp++;
			}
		}
	}
	delete[] traversed_index;

	for (int i = 0; i < number_of_nodes; i++) {
		path_to_node[i] = old_path[i];
	}
	delete[] finished_words;
	delete[] old_path;

	return new_words;
}

struct stack {

	string* stack_of_strings;
	int top_of_stack;
	int size = 1;

	stack() {
		stack_of_strings = new string[size];
		top_of_stack = 0;
	}

	void push(string node) {
		if (top_of_stack == size) {
			size *= 2;
			string* new_stack_of_strings = new string[size];
			for (int i = 0; i < top_of_stack; i++) {
				new_stack_of_strings[i] = stack_of_strings[i];
			}
			delete[] stack_of_strings;
			stack_of_strings = new_stack_of_strings;
		}
		stack_of_strings[top_of_stack] = node;
		top_of_stack++;
	}

	string pop() {
		string x = stack_of_strings[top_of_stack - 1];
		top_of_stack--;

		if (top_of_stack < size / 2 && size != 1) {
			size /= 2;
			string* new_stack_of_strings = new string[size];
			for (int i = 0; i < top_of_stack; i++) {
				new_stack_of_strings[i] = stack_of_strings[i];
			}
			delete[] stack_of_strings;
			stack_of_strings = new_stack_of_strings;
		}
		return x;
	}
	int empty_stack() {
		if (top_of_stack == 0) {
			return 1;
		}
		return 0;
	}

	void print_stack() {
		for (int i = 0; i < top_of_stack; i++) {
			cout << stack_of_strings[i] << " ";
		}
		cout << "\n";
	}

};

void dfs_visit(string word, int number_of_nodes, string* nodes, double** matrix_of_graph, string* not_visited_nodes) {

	int* visit = new int[number_of_nodes];
	stack stack_of_strings;
	string node = word;
	int index;

	for (int i = 0; i < number_of_nodes; i++) {
		not_visited_nodes[i] = "";
		visit[i] = 0;
	}

	stack_of_strings.push(node);

	while (!stack_of_strings.empty_stack()) {
		node = stack_of_strings.pop();
		index = find_index_of_word_in_array(nodes, node, number_of_nodes);
		if (visit[index] == 0) {
			visit[index] = 1;
			for (int j = 0; j < number_of_nodes; j++) {
				if (matrix_of_graph[index][j] != 0) {
					if (visit[j] == 0) {
						stack_of_strings.push(nodes[j]);
					}
				}
			}
		}
	}

	int number_of_not_visited_nodes = 0;
	for (int i = 0; i < number_of_nodes; i++) {
		if (visit[i] == 0) {
			not_visited_nodes[number_of_not_visited_nodes] = nodes[i];
			number_of_not_visited_nodes++;
		}
	}
	delete[] visit;
}

string* dfs_find_connected_words(string word, int number_of_nodes, string* nodes, double** matrix_of_graph, string* not_visited_nodes) {

	int* visit = new int[number_of_nodes];
	stack stack_of_strings;
	string node = word;
	int index;
	int number_of_connected_words = 0;
	string* strong_connected_words = new string[number_of_nodes];

	for (int i = 0; i < number_of_nodes; i++) {
		strong_connected_words[i] = "";
		visit[i] = 0;
	}

	stack_of_strings.push(node);

	while (!stack_of_strings.empty_stack()) {
		node = stack_of_strings.pop();
		index = find_index_of_word_in_array(nodes, node, number_of_nodes);
		if (visit[index] == 0) {
			visit[index] = 1;
			strong_connected_words[number_of_connected_words] = nodes[index];
			number_of_connected_words++;
			for (int j = 0; j < number_of_nodes; j++) {
				if (matrix_of_graph[index][j] != 0 && !find_word(nodes[j], not_visited_nodes, number_of_nodes)) {
					if (visit[j] == 0) {
						stack_of_strings.push(nodes[j]);
					}
				}
			}
		}
	}
	delete[] visit;
	return strong_connected_words;
}

string* find_connected_words(string word, int number_of_nodes, string* nodes, double** matrix_of_graph) {

	string* strong_connected_words;
	string node = word;
	int index;
	string* not_visited_nodes = new string[number_of_nodes];
	double** transpose_matrix_of_graph;

	dfs_visit(word, number_of_nodes, nodes, matrix_of_graph, not_visited_nodes);

	transpose_matrix_of_graph = (double**)malloc(number_of_nodes * sizeof(double*));
	for (int i = 0; i < number_of_nodes; i++) {
		transpose_matrix_of_graph[i] = (double*)malloc(number_of_nodes * sizeof(double));
	}

	for (int i = 0; i < number_of_nodes; i++) {
		for (int j = 0; j < number_of_nodes; j++) {
			transpose_matrix_of_graph[i][j] = matrix_of_graph[j][i];
		}
	}

	strong_connected_words = dfs_find_connected_words(word, number_of_nodes, nodes, transpose_matrix_of_graph, not_visited_nodes);

	delete[] not_visited_nodes;

	for (int i = 0; i < number_of_nodes; i++) {
		free(transpose_matrix_of_graph[i]);
	}
	free(transpose_matrix_of_graph);

	return strong_connected_words;
}

int main() {


	int option;
	int way_of_enter_graph = 0;
	string name_of_file;
	ifstream file;
	int number_of_nodes = 0;
	int number_of_edges = 0;
	string line;
	string* nodes = NULL;
	double** matrix_of_graph = NULL;
	string new_node;
	string old_node;
	string new_edge;
	string old_egde;
	double* path_to_node = NULL;
	string* previous_nodes = NULL;
	string* k_similar_words = NULL;
	int k;
	string node1;
	string node2;
	int flag_end_of_program = 0;
	string* strong_connected_words = NULL;

	cout << "Pocetak programa" << "\n";

	while (true) {
		cout << "1. Ucitati graf" << "\n";
		cout << "2. Dodavanje cvora u graf" << "\n";
		cout << "3. Uklanjanje cvora iz graf" << "\n";
		cout << "4. Dodavanje grane u graf izmedju dva cvora u grafu" << "\n";
		cout << "5. Uklanjanje grane izmedju dva cvora u grafu" << "\n";
		cout << "6. Ispis reprezentacije grafa" << "\n";
		cout << "7. Brisanje grafa iz memorije" << "\n";
		cout << "8. Nalazenje k semanticki najslicnijih reci" << "\n";
		cout << "9. Ispis svih reci na najkracem putu od jedne do druge reci" << "\n";
		cout << "10. Nalazenje svih reci koje su jako povezane sa zadatom recju" << "\n";
		cout << "11. Kraj programa" << "\n";

		cin >> option;

		switch (option) {

		case 1: {

			cout << "1. Unesite graf uz pomoc tekstualne datoteke" << "\n";
			cout << "2. Unesite graf preko standardnog ulaza" << "\n";
			cin >> way_of_enter_graph;
			if (way_of_enter_graph == 1) {
				cout << "Unesite putanju ka fajlu" << "\n";
				cin.ignore();
				getline(cin, name_of_file);
				file.open(name_of_file);
				if (!file.is_open()) {
					cout << "Uneli ste pogresnu adresu ka fajlu" << "\n";
					continue;
				}

				getline(file, line);
				number_of_nodes = read_number_of_nodes(line);

				getline(file, line);
				number_of_edges = read_number_of_edges(line);

				getline(file, line);
				nodes = read_words_from_file(line, number_of_nodes);

				matrix_of_graph = make_a_matrix_of_graph(number_of_nodes, file, nodes);
				path_to_node = new double[number_of_nodes];
				previous_nodes = new string[number_of_nodes];

			}
			if (way_of_enter_graph == 2) {

				cout << "Unesite broj cvorova u grafu" << "\n";
				cin.ignore();
				cin >> number_of_nodes;
				cout << "Unesite broj grana u grafu" << "\n";
				cin.ignore();
				cin >> number_of_edges;
				cout << "\n";
				nodes = new string[number_of_nodes];
				for (int i = 0; i < number_of_nodes; i++) {
					cout << "Unesite cvor" << "\n";
					if (i == 0) {
						cin.ignore();
					}
					getline(cin, nodes[i]);
				}
				matrix_of_graph = (double**)malloc(number_of_nodes * sizeof(double*));
				for (int i = 0; i < number_of_nodes; i++) {
					matrix_of_graph[i] = (double*)malloc(number_of_nodes * sizeof(double));
				}

				for (int i = 0; i < number_of_nodes; i++) {
					for (int j = 0; j < number_of_nodes; j++) {
						matrix_of_graph[i][j] = 0;
					}
				}

				for (int i = 0; i < number_of_nodes; i++) {
					cout << nodes[i] << " ";
				}
				int count = 0;
				while (true) {
					cout << "Unesite granu grafa" << "\n";
					string edge;
					getline(cin, edge);
					if (add_edge_in_graph(edge, number_of_nodes, count, nodes, matrix_of_graph) == 1) {
						if (count == number_of_edges) {
							break;
						}
					}
				}

				for (int i = 0; i < number_of_nodes; i++) {
					for (int j = 0; j < number_of_nodes; j++) {
						cout << matrix_of_graph[i][j] << " ";
					}
					cout << "\n";
				}
				cout << "\n";
				path_to_node = new double[number_of_nodes];
				previous_nodes = new string[number_of_nodes];
			}
			break;
		}
		case 2: {
			cout << "Unesite cvor koji zelite da unesete u graf" << "\n";
			cin.ignore();
			getline(cin, new_node);
			nodes = add_node_in_graph(new_node, number_of_nodes, nodes, matrix_of_graph);
			break;
		}
		case 3: {
			cout << "Unesite koji cvor zelite da izbrisete iz grafa" << "\n";
			cin.ignore();
			getline(cin, old_node);
			nodes = remove_node_from_graph(old_node, number_of_nodes, number_of_edges, nodes, matrix_of_graph);
			break;
		}
		case 4: {
			cout << "Unesite granu koju zelite da unesete u graf" << "\n";
			cin.ignore();
			getline(cin, new_edge);
			add_edge_in_graph(new_edge, number_of_nodes, number_of_edges, nodes, matrix_of_graph);
			break;
		}
		case 5: {
			cout << "Unesite granu koju zelite da izbrisete iz grafa(cvor1 cvor2)" << "\n";
			cin.ignore();
			getline(cin, old_egde);
			remove_edge_in_graph(old_egde, number_of_nodes, number_of_edges, nodes, matrix_of_graph);
			break;
		}
		case 6: {
			cout << "Matricna predstava grafa" << "\n";
			represent_graph(nodes, matrix_of_graph, number_of_nodes);
			break;
		}
		case 7: {
			delete_memory(nodes, matrix_of_graph, number_of_nodes, path_to_node, previous_nodes, k_similar_words, strong_connected_words);
			break;
		}
		case 8: {
			if (k_similar_words != NULL) {
				delete[] k_similar_words;
			}
			if (path_to_node != NULL) {
				delete[] path_to_node;
			}
			if (previous_nodes != NULL) {
				delete[] previous_nodes;
			}
			path_to_node = new double[number_of_nodes];
			previous_nodes = new string[number_of_nodes];

			cout << "Unesite za koju rec trazimo k semanticki najslicnijih reci:" << "\n";
			cin.ignore();
			getline(cin, line);
			cout << "\n";
			cout << "Unesite vrednost za k(broj semanticki najslicnijih reci):" << "\n";
			cin >> k;
			k_similar_words = find_k_similar_words(line, nodes, matrix_of_graph, number_of_nodes, k, path_to_node, previous_nodes);
			for (int i = 0; i < k; i++) {
				cout << k_similar_words[i] << " ";
			}
			cout << "\n";
			break;
		}
		case 9: {
			cout << "Unesite od kog cvora trazimo najkracu putanju" << "\n";
			cin.ignore();
			getline(cin, node1);
			cout << "\n";
			cout << "Unesite do kog cvora trazimo najkracu putanju" << "\n";
			getline(cin, node2);
			cout << "\n";

			if (path_to_node != NULL) {
				delete[] path_to_node;
			}
			if (previous_nodes != NULL) {
				delete[] previous_nodes;
			}
			path_to_node = new double[number_of_nodes];
			previous_nodes = new string[number_of_nodes];

			find_k_similar_words(node1, nodes, matrix_of_graph, number_of_nodes, 5, path_to_node, previous_nodes);
			find_the_shortest_path(previous_nodes, number_of_nodes, matrix_of_graph, nodes, node1, node2);
			break;
		}
		case 10: {
			if (strong_connected_words != NULL) {
				delete[] strong_connected_words;
			}
			cout << "Unesite rec za koju je potrebno pronaci jaku povezanost sa ostalim recima" << "\n";
			cin.ignore();
			getline(cin, line);
			strong_connected_words = find_connected_words(line, number_of_nodes, nodes, matrix_of_graph);
			for (int i = 0; i < number_of_nodes; i++) {
				if (strong_connected_words[i] == "") {
					break;
				}
				else {
					cout << strong_connected_words[i] << " ";
				}

			}
			cout << "\n";
			break;
		}
		case 11: {
			flag_end_of_program = 1;
			delete_memory(nodes, matrix_of_graph, number_of_nodes, path_to_node, previous_nodes, k_similar_words, strong_connected_words);
			break;
		}
		}

		if (flag_end_of_program) {
			break;
		}

	}

	return 0;
}


