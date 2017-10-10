#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

bool occurenceMatch(string str, int noOccurence){ // to find number of occurences of comma (USED FOR ENTRY VALIDATION)
	int counter = 0;
	int number = 0;
	while (counter < str.length()){
		if (str[counter] == ','){
			number++;
		}
		counter++;
	}
	if (number == noOccurence){
		return true;
	}
	return false;
}

class ColumnNode{
	friend class RowNode;
	friend class SM;
private:
	ColumnNode *nextCol; // pointer to next column
	int colNo, value; // column number and value
	ColumnNode(int colNo, int val); // constructor to set value and column number
};
ColumnNode::ColumnNode(int colNo, int val){
	value = val;
	this->colNo = colNo;
	nextCol = NULL;
}
class RowNode{
	friend class SM;
private:
	int rowNo, colSize; // row number & actual number of column nodes inside this row node
	ColumnNode *colHead, *colTail; // pointer to the first and last column inside this row node
	RowNode *nextRow; // pointer to next row node
	RowNode(){}
	RowNode(int rowNo);
	~RowNode();
	void deleteFirstCol(); //deletes the first column node and assigns the next to head
	void overwriteColumn(ColumnNode *curr, int val); // if a column node exists prompts to overwrite the value
	void insertColumn(int colIndex, int val); //inserting column inside a row node
	void printCol(int mSize); // printing column nodes inside a row node
};
RowNode::RowNode(int rowNo){
	this->rowNo = rowNo;
	nextRow = NULL;
	colHead = NULL;
	colTail = NULL;
}
RowNode::~RowNode(){
	while (colHead != NULL){ //while loop to delete the first column until there is no column node left
		deleteFirstCol();
	}
}
void RowNode::deleteFirstCol(){
	ColumnNode *toBeDeleted = colHead;
	colHead = colHead->nextCol;
	delete toBeDeleted;
	if (colHead == NULL){
		colTail = NULL;
	}
}
void RowNode::overwriteColumn(ColumnNode *curr, int val){
	cout << "A value already exists in row no. " << rowNo << " and col no. " << curr->colNo << endl << "Would you like to overwrite? (Y/N) ";
	char oW;
	cin >> oW;
	if (oW == 'Y'){
		curr->value = val;
		cout << "Value has been overwritten." << endl;
	}
	else{
		cout << "Value has not been overwritten." << endl;
	}
}
void RowNode::insertColumn(int colIndex, int val){
	if (colHead == NULL || (colTail != NULL && colIndex > colTail->colNo)){ //if the list is empty or column number is greater than the last column number in the current list
		ColumnNode *newNode = new ColumnNode(colIndex, val);
		if (colHead == NULL){ //if the list is empty
			colHead = colTail = newNode;
		}
		else{
			colTail->nextCol = newNode;
			colTail = newNode;
		}
		colSize++;
		return;
	}
	if (colTail->colNo == colIndex){ //if the column number is the same as last column number, prompts to overwrite
		overwriteColumn(colTail, val);
		return;
	}
	if (colIndex < colHead->colNo){//insert at beginning
		ColumnNode *newCol = new ColumnNode(colIndex, val);
		newCol->nextCol = colHead;
		colHead = newCol;
		colSize++;
		return;
	}
	ColumnNode *curr = colHead;
	for (int i = 0; i < colSize; i++){
		if (curr->colNo == colIndex){ //if column node is found, then prompt overwrite
			overwriteColumn(curr, val);
			break;
		}
		else if (curr->nextCol != NULL){
			if (curr->colNo < colIndex && curr->nextCol->colNo > colIndex){//row node can fit between two other nodes (for ex. 4 can fit between 3 5)
				ColumnNode *newCol = new ColumnNode(colIndex, val);
				newCol->nextCol = curr->nextCol;
				curr->nextCol = newCol;
				colSize++;
				break;
			}
			else{
				curr = curr->nextCol;
			}
		}
	}
}
void RowNode::printCol(int mSize){
	int counter = 0;
	ColumnNode *curr = colHead;
	while (counter < mSize){
		/*
		iterates using a counter which will indicate the column size
		if the counter matches the column number then it prints the value and points to the next column
		otherwise it does not exist, so it prints a zero
		if the current column is null and the counter is still less than column size
		then it will have to print zero's for the rest of the iterations
		*/
		if (curr != NULL){
			if (counter == curr->colNo){
				cout << right << setw(5) << curr->value;
				curr = curr->nextCol;
			}
			else{
				cout << setw(5) << right << "0";
			}
		}
		else{
			cout << setw(5) << right << "0";
		}
		counter++;
	}
}
class SM{
private:
	int n, m; // max number of rows and columns
	int rowSize; // actual size of the row list
	RowNode *rowHead; //pointer to the first row in the list
	RowNode *rowTail; //pointer to the last row in the list
	void insertEntry(int rowIndex, int colIndex, int val); // inserting a new element into a row x column
	void deleteFirstRow();  // deletes the first row node in the list and assigns the next to head
	void printColumnHeader();// will print out the column numbers ( ex 1 2 3 4 5 6 7 8 9 10)
	void printEmptyRow(int counter); // will print out an empty row for the counter number (ex 5| 0 0 0 0 0 0)

public:
	SM(int rows, int columns);
	~SM();
	void readElements();
	void printMatrix();
	SM * addSM(SM &other);
};
void SM::insertEntry(int rowIndex, int colIndex, int val){
	if (rowTail == NULL || rowIndex > rowTail->rowNo){ //insert at end if the list is empty or the row number is greater than the last row number
		RowNode *newNode = new RowNode(rowIndex);
		if (rowHead == NULL){ // if the list is empty
			rowHead = rowTail = newNode;
		}
		else{
			rowTail->nextRow = newNode;
			rowTail = newNode;
		}
		rowSize++;
		newNode->insertColumn(colIndex, val);
		return;
	}
	if (rowTail->rowNo == rowIndex){ //if the row number is equal to the last row's number then insert column at end
		rowTail->insertColumn(colIndex, val);
		return;
	}
	if (rowIndex < rowHead->rowNo){ //insert at beginning
		RowNode *newRow = new RowNode(rowIndex);
		newRow->nextRow = rowHead;
		rowHead = newRow;
		rowSize++;
		newRow->insertColumn(colIndex, val);
		return;
	}
	RowNode *curr = rowHead;
	for (int i = 0; i < rowSize; i++){
		if (curr->rowNo == rowIndex){ //if row node already exists
			curr->insertColumn(colIndex, val);
			break;
		}
		else if (curr->nextRow != NULL){
			if (curr->rowNo < rowIndex && curr->nextRow->rowNo > rowIndex){//row node can fit between two other nodes (for ex. 4 can fit between 3 5)
				RowNode *newRow = new RowNode(rowIndex);
				newRow->nextRow = curr->nextRow;
				curr->nextRow = newRow;
				rowSize++;
				newRow->insertColumn(colIndex, val);
				break;
			}
			else{
				curr = curr->nextRow;
			}
		}
	}
}
void SM::deleteFirstRow(){
	RowNode *toBeDeleted = rowHead;
	rowHead = rowHead->nextRow;
	delete toBeDeleted;
	if (rowHead == NULL){
		rowTail = NULL;
	}
}
void SM::printColumnHeader(){
	cout << "-----------------------------------------------------------" << endl;
	cout << "Sparse-matrix Visual Representation: " << endl << endl;
	cout << " R|C";

	for (int i = 0; i < m; i++){
		cout << right << setw(5) << i;
	}
	cout << endl;
	for (int i = 0; i < m * 6; i++){
		cout << right << "-";
	}
	cout << endl;
}
void SM::printEmptyRow(int counter){
	for (int i = 0; i < m; i++){
		cout << setw(5) << right << "0";
	}
	cout << endl;
}
SM::SM(int rows, int columns){
	n = rows;
	m = columns;
	rowHead = NULL;
	rowTail = NULL;
	rowSize = 0;
	cout << "A zero-based Sparse Matrix has been created with the following dimensions, Row(s) " << n << " and Column(s) " << m << endl;
}
SM::~SM(){
	while (rowHead != NULL){ //while there are still rows left(row head is not null) keep deleting the first row
		deleteFirstRow();
	}

	cout << "Sparse Matrix has been destroyed" << endl;
}
void SM::readElements(){
		{
			string input = "";
			cout << "Please input your desired elements into the sparse matrix \n";
			cout << "Use the format row index,column index,value (EXAMPLE 0,1,1)" << endl;
			cout << "Type EXIT to stop inputting" << endl;
			while (1){
				int rowIndex, colIndex, val;
				cout << "Input: ";
				cin >> input;
				string toLower;
				for (int i = 0; i < input.length(); i++){
					toLower += tolower(input[i]);
				}
				if (toLower == "exit"){
					break;
				}
				else{
					try{
						if (!(occurenceMatch(input, 2))){
							throw exception();
						}
						rowIndex = stoi(input.substr(0, input.find_first_of(',')));
						colIndex = stoi(input.substr(input.find_first_of(',') + 1, input.find_last_of(',')));
						val = stoi(input.substr(input.find_last_of(',') + 1, input.length()));
						if (colIndex < 0 || colIndex >= m || rowIndex < 0 || rowIndex >= n){
							cout << "Out of bound. Maximum Column number entry is " << m - 1 << " and Maximum Row number entry is " << n - 1 << endl;
						}
						else if (val == 0){
							cout << "Value must be non-zero!" << endl;
						}
						else
							insertEntry(rowIndex, colIndex, val);

					}
					catch (exception e){
						cout << "Invalid input, please try again" << endl;
					}
				}
			}
		}
}
SM * SM::addSM(SM &other){
		{
			if ((other.m == this->m) && (other.n == this->n)){ // check if dimensions of 2 matrices are equal
				SM * newSM = new SM(this->n, this->m); // create a new sparse matrix
				RowNode * thisCurrRow = this->rowHead;  // pointer to first row node in this SM
				RowNode * otherCurrRow = other.rowHead; // pointer to first row node in other SM
				ColumnNode *otherCurrCol = NULL; // pointer to a column node in other SM (to be used later)
				ColumnNode *thisCurrCol = NULL; // pointer to a column node in this SM (to be used later)
				while (thisCurrRow != NULL || otherCurrRow != NULL){
					/*
					There are 3 scenarios for EACH SM that need to be dealt with
					1. This SM row pointer is NULL, so insert all the other SM's row nodes and column nodes (same needs to be replicated for other SM)
					2. This SM row  pointer's number matches other SM row number, so there are several scenarios to be dealth with
					2.1. Both column number's match, so insert new entry with the sum of those two numbers
					2.2. This SM column number is less than the other's then insert this column number (same replicated for other SM)
					2.3. This SM's column pointer is null, so insert all of other's column node(meaning that it might have reached the end of column list for this SM, but otherSM still has column nodes)
					3. This SM row pointer's number is less than the other SM row number, so insert all the rows until it is no longer less than other SM row number and it is not NULL (hasn't reached end of list)
					IMPORTANT: EACH OF THESE HAS BEEN REPLICIATED FOR BOTH THIS SM AND OTHER SM, SINCE THEY BOTH HAVE SAME SCENARIOS WHICH NEED TO BE TAKEN CARE OF
					*/
					if (thisCurrRow == NULL && otherCurrRow != NULL){
						while (otherCurrRow != NULL){
							otherCurrCol = otherCurrRow->colHead;
							while (otherCurrCol != NULL){
								newSM->insertEntry(otherCurrRow->rowNo, otherCurrCol->colNo, otherCurrCol->value);
								otherCurrCol = otherCurrCol->nextCol;
							}
							otherCurrRow = otherCurrRow->nextRow;
						}
						break;
					}
					else if (thisCurrRow != NULL && otherCurrRow == NULL){
						while (thisCurrRow != NULL){
							thisCurrCol = thisCurrRow->colHead;
							while (thisCurrCol != NULL){
								newSM->insertEntry(thisCurrRow->rowNo, thisCurrCol->colNo, thisCurrCol->value);
								thisCurrCol = thisCurrCol->nextCol;
							}
							thisCurrRow = thisCurrRow->nextRow;
						}
					}
					else if (thisCurrRow->rowNo == otherCurrRow->rowNo){
						thisCurrCol = thisCurrRow->colHead;
						otherCurrCol = otherCurrRow->colHead;
						while (thisCurrCol != NULL || otherCurrCol != NULL){
							if (otherCurrCol == NULL){
								while (thisCurrCol != NULL){
									newSM->insertEntry(thisCurrRow->rowNo, thisCurrCol->colNo, thisCurrCol->value);
									thisCurrCol = thisCurrCol->nextCol;
								}
							}
							else if (thisCurrCol == NULL){
								while (otherCurrCol != NULL){
									newSM->insertEntry(otherCurrRow->rowNo, otherCurrCol->colNo, otherCurrCol->value);
									otherCurrCol = otherCurrCol->nextCol;
								}
							}
							else if (thisCurrCol->colNo == otherCurrCol->colNo){
								int sum = thisCurrCol->value + otherCurrCol->value;
								newSM->insertEntry(thisCurrRow->rowNo, thisCurrCol->colNo, sum);
								thisCurrCol = thisCurrCol->nextCol;
								otherCurrCol = otherCurrCol->nextCol;
							}
							else if (thisCurrCol->colNo < otherCurrCol->colNo){
								newSM->insertEntry(thisCurrRow->rowNo, thisCurrCol->colNo, thisCurrCol->value);
								thisCurrCol = thisCurrCol->nextCol;
							}

							else if (otherCurrCol->colNo < thisCurrCol->colNo){
								newSM->insertEntry(otherCurrRow->rowNo, otherCurrCol->colNo, otherCurrCol->value);
								otherCurrCol = otherCurrCol->nextCol;
							}
						}
						thisCurrRow = thisCurrRow->nextRow;
						otherCurrRow = otherCurrRow->nextRow;

					}
					else if (thisCurrRow->rowNo < otherCurrRow->rowNo){
						while (thisCurrRow != NULL && thisCurrRow->rowNo < otherCurrRow->rowNo){
							thisCurrCol = thisCurrRow->colHead;
							while (thisCurrCol != NULL){
								newSM->insertEntry(thisCurrRow->rowNo, thisCurrCol->colNo, thisCurrCol->value);
								thisCurrCol = thisCurrCol->nextCol;
							}
							thisCurrRow = thisCurrRow->nextRow;
						}
					}
					else if (otherCurrRow->rowNo < thisCurrRow->rowNo){
						while (otherCurrRow != NULL && otherCurrRow->rowNo < thisCurrRow->rowNo){
							otherCurrCol = otherCurrRow->colHead;
							while (otherCurrCol != NULL){
								newSM->insertEntry(otherCurrRow->rowNo, otherCurrCol->colNo, otherCurrCol->value);
								otherCurrCol = otherCurrCol->nextCol;
							}
							otherCurrRow = otherCurrRow->nextRow;
						}
					}

				}
				return newSM;
			}
			else
				cout << "The sparse matrices dimensions are not the same. Aborting...";
			system("pause");
			abort();
		}
}
void SM::printMatrix(){
	printColumnHeader();
	int counter = 0;
	RowNode * curr = rowHead;
	while (counter < n){
		/*
		while loop with a counter to iterate through the maximum number of rows
		if the counter matches the current row number then it prints the columns inside that row and points to next
		otherwise it prints out empty
		if the current row is null and the counter hasn't reached the end
		then it will print zeros for the rest of the iterations
		*/
		cout << setw(3) << counter << "|";
		if (curr != NULL){
			if (counter == curr->rowNo){
				curr->printCol(m);
				curr = curr->nextRow;
				cout << endl;
			}
			else
				printEmptyRow(counter);
		}
		else
			printEmptyRow(counter);
		counter++;
	}
	cout << endl << "-----------------------------------------------------------" << endl;
}
int main(){
	int r1, c1, r2, c2;
	string in;
	cout << "Please enter the sparse matrix 1 dimensions in the form row,column" << endl
		 << "For example 5,10 for 5 rows and 10 columns" << endl;
	cout << "Input: ";
	cin >> in;
	try{
		if (!occurenceMatch(in, 1)){
			cout << "Invalid input" << endl;
			system("pause");
			return 0;
		}
		r1 = stoi(in.substr(0, in.find_first_of(',')));
		c1 = stoi(in.substr(in.find_first_of(',') + 1, in.length()));
		if (!(r1 > 0 && c1 > 0)){
			cout << "ABORTING: Row and Column dimensions must both be greater than 0!" << endl;
			abort();
		}
		SM *s = new SM(r1, c1);
		s->readElements();
		s->printMatrix();
		system("pause");
		system("cls");
		cout << "Please enter the Sparse Matrix 2 dimensions in the form row,column" << endl
			 << "For example 5,10 for 5 rows and 10 columns" << endl;
		cout << "Input: ";
		cin >> in;
		if (!occurenceMatch(in, 1)){
			cout << "Invalid input" << endl;
			system("pause");
			return 0;
		}
		r2 = stoi(in.substr(0, in.find_first_of(',')));
		c2 = stoi(in.substr(in.find_first_of(',') + 1, in.length()));
		if (!(r2 > 0 && c2 > 0)){
			cout << "ABORTING: Row and Column dimensions must both be greater than 0!" << endl;
			abort();
		}
		SM *d = new SM(r2, c2);
		d->readElements();
		d->printMatrix();
		system("pause");
		system("cls");
		cout << "Sparse Matrix 1." << endl;
		s->printMatrix();
		cout << "Sparse Matrix 2." << endl;
		d->printMatrix();
		SM *f = s->addSM(*d);
		cout << "Sum of Matrix 1 & 2" << endl;
		f->printMatrix();
		delete s;
		delete d;
		delete f;
	}
	catch (exception ex){
		cout << "Invalid input, aborting." << endl;
	}
	system("pause");
	return 0;
}
