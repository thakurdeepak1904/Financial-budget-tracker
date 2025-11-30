#include <iostream>
#include <vector>
#include <string>
#include <ctime> // For time functions
#include <fstream>
#include <exception>
using namespace std;

class Transaction
{
public:
    static int nextID; // static member to keep track of next ID
    int id;
    double amount;
    string description;
    string date;        // (YYYY-MM-DD) auto generated
    string category;    // (income/expense)
    string subcategory; // (income types/expense types)

    static string getCurrentDate()
    {
        time_t now = time(0); // get current time
        tm *ltm = localtime(&now);
        char time[11]; // YYYY-MM-DD\0 (11 chars needed)
        strftime(time, sizeof(time), "%Y-%m-%d", ltm);

        return string(time);
    }

    Transaction() {} // default constructor
    Transaction(double a, string d, string c, string sc)
    {
        id = nextID++;
        amount = a;
        description = d;
        date = getCurrentDate();
        category = c;
        subcategory = sc;
    }

    static Transaction readFromFile(ifstream &in)
    {
        Transaction t;
        string amount;
        try
        {
            getline(in, amount);
            if (amount == "")
                getline(in, amount);
            t.id = stoi(amount);

            getline(in, amount);
            t.amount = stod(amount);

            getline(in, t.description);
            getline(in, t.date);
            getline(in, t.category);
            getline(in, t.subcategory);
        }
        catch (const exception &e)
        {
            // Handle any conversion errors or unexpected end of file
            cout << "Error reading transaction from file: " << e.what() << endl;
            cout << "Skipping to next transaction." << endl;

            // return a default empty transaction
            t.id = -1;
        }
        return t;
    }
};

int Transaction::nextID = 1;

// custom exception class for input validation
class InputException : public exception
{
    string message;

public:
    InputException(const string &msg) : message(msg) {}

    const char *what() const noexcept override
    {
        return message.c_str();
    }
};
// main menu display function
void displayMenu()
{
    cout << "\n===============================" << endl;
    cout << "Welcome to the Budget Tracker" << endl;
    cout << "===============================" << endl;
    cout << "Please Select an Option from the Menu Below:" << endl;
    cout << "1. Add a new transaction" << endl;
    cout << "2. Delete an existing transaction" << endl;
    cout << "3. Display all transactions" << endl;
    cout << "4. Sort transactions" << endl;
    cout << "5. Search for a transaction" << endl;
    cout << "6. Save all transactions to file" << endl;
    cout << "7. Load transactions from file" << endl;
    cout << "8. Exit" << endl;
    cout << "Enter your choice: ";
}

// input validation function
int inputValidation()
{
    string input;
    int value;
    while (true)
    {
        cin >> input;
        try
        {
            value = stoi(input);
            return value;
        }
        catch (exception &)
        {
            cout << "---------------------------------" << endl;
            throw InputException("Invalid integer entered.");
        }
    }
}
// input validation for double(amount)
double inputValidationDouble()
{
    string input;
    double value;
    while (true)
    {
        cin >> input;
        try
        {
            value = stod(input);
            return value;
        }
        catch (exception &)
        {
            cout << "---------------------------------" << endl;
            throw InputException("Invalid number entered.");
        }
    }
}
// expense type lists
void expense()
{
    cout << "\n"
         << endl;
    cout << "Select expense type:" << endl;
    cout << "1. Rent:" << endl;
    cout << "2. Grocery:" << endl;
    cout << "3. Insurance:" << endl;
    cout << "4. Entertainment:" << endl;
    cout << "5. Shopping" << endl;
    cout << "6. Others:" << endl;
    cout << "Enter your choice for expense type: ";
}
// income type lists
void income()
{
    cout << "Select income type:" << endl;
    cout << "1. Salary:" << endl;
    cout << "2. Bonus:" << endl;
    cout << "3. Interest/ Dividend:" << endl;
    cout << "4. Others:" << endl;
    cout << "Enter your choice for income type: ";
}

// add transaction function
Transaction addTransaction()
{

    double amount;
    string description;
    string date;
    string category;
    string subcategory = "";
    int subChoice;
    cout << "\n ----------------------------- \n"
         << endl;
    cout << "You selected Add a new transaction." << endl;
    cout << "ID: " << Transaction::nextID << endl;
    do
    {
        cout << "Enter Amount: ";
        while (true)
        {
            try
            {
                amount = inputValidationDouble();
                break; // exit loop if input is valid
            }
            catch (InputException &e)
            {
                cout << e.what() << endl;
                cout << "Please enter the amount again: ";
            }
        }
        if (amount <= 0)
        {
            cout << "Amount must be positive. Please try again." << endl;
        }
        else
        {
            break;
        }

    } while (amount <= 0);
    cout << "Enter Description: ";
    cin.ignore();
    getline(cin, description);
    cout << "Date: " << Transaction::getCurrentDate() << endl;

    do
    {
        cout << "Enter Category (income/expense): ";
        cin >> category;
        if (category != "income" && category != "expense")
        {
            cout << "Invalid category. Please enter 'income' or 'expense'." << endl;
        }
    } while (category != "income" && category != "expense");

    // subcategory selection
    if (category == "income")
    {
        income();
        while (true)
        {
            try
            {
                subChoice = inputValidation();
                break;
            }
            catch (InputException &e)
            {
                cout << e.what() << endl;
                cout << "Enter a valid option: ";
            }
        }

        switch (subChoice)
        {
        case 1:
            subcategory = "Salary";
            break;
        case 2:
            subcategory = "Bonus";
            break;
        case 3:
            subcategory = "Interest/ Dividend";
            break;
        default:
            subcategory = "Others";
            break;
        }
    }

    else if (category == "expense")
    {
        expense();
        cout << "Enter your choice for expense type: ";
        while (true)
        {
            try
            {
                subChoice = inputValidation();
                break;
            }
            catch (InputException &e)
            {
                cout << e.what() << endl;
                cout << "Enter a valid option: ";
            }
        }
        switch (subChoice)
        {
        case 1:
            subcategory = "Rent";
            break;
        case 2:
            subcategory = "Grocery";
            break;
        case 3:
            subcategory = "Insurance";
            break;
        case 4:
            subcategory = "Entertainment";
            break;

        case 5:
            subcategory = "Shopping";
            break;

        default:
            subcategory = "Others";
            break;
        }
    }
    return Transaction(amount, description, category, subcategory);
}

// delete transaction by ID function
void deleteTransactionbyID(vector<Transaction> &transactions)
{
    cout << "\n -------- Delete a transaction -------- \n"
         << endl;
    if (transactions.empty())
    {
        cout << "No transactions available to delete." << endl;
        return;
    }

    int idToDelete = 0;
    cout << "Enter the Transaction ID to delete: ";

    while (true)
    {
        try
        {
            idToDelete = inputValidation();
            break; // exit loop if input is valid
        }
        catch (InputException &e)
        {
            cout << e.what() << endl;
            cout << "Please enter the ID again: ";
        }
    }
    if (idToDelete <= 0)
    {
        cout << "Invalid ID. ID must be a positive integer." << endl;
        return;
    }
    for (int i = 0; i < transactions.size(); i++)
    {
        if (transactions[i].id == idToDelete)
        {
            transactions.erase(transactions.begin() + i);
            cout << "Transaction with ID " << idToDelete << " has been deleted." << endl;
            return;
        }
    }
    cout << "Transaction with ID " << idToDelete << " not found." << endl;
}

// Delete Transaction by Amount function
void deleteTransactionbyAmount(vector<Transaction> &transactions)
{
    cout << "\n -------- Delete a transaction -------- \n"
         << endl;
    if (transactions.empty())
    {
        cout << "No transactions available to delete." << endl;
        return;
    }

    double amountToDelete;
    cout << "Enter the Transaction Amount to delete: ";
    while (true)
    {
        try
        {
            amountToDelete = inputValidationDouble();
            break; // exit loop if input is valid
        }
        catch (InputException &e)
        {
            cout << e.what() << endl;
            cout << "Please enter the amount again: ";
        }
    }
    for (int i = 0; i < transactions.size(); i++)
    {
        if (transactions[i].amount == amountToDelete)
        {
            transactions.erase(transactions.begin() + i);
            cout << "Transaction with Amount " << amountToDelete << " has been deleted." << endl;
            return;
        }
    }
    cout << "Transaction with Amount " << amountToDelete << " not found." << endl;
}

// sorting functions
void bubbleSortForAmount(vector<Transaction> &t)
{
    int n = t.size();
    for (int i = 0; i < n - 1; i++)
    {
        bool flag = false;
        for (int j = 0; j < n - i - 1; j++)
        {
            if (t[j].amount > t[j + 1].amount)
            {
                swap(t[j], t[j + 1]);
                // Setting flag true when swap happened
                flag = true;
            }
        }
        // Checking if the flag is set or not
        if (!flag)
            break;
    }
}
void bubbleSortForDate(vector<Transaction> &t)
{
    int n = t.size();
    for (int i = 0; i < n - 1; i++)
    {
        bool flag = false;
        for (int j = 0; j < n - i - 1; j++)
        {
            if (t[j].date > t[j + 1].date)
            {
                swap(t[j], t[j + 1]);
                // Setting flag true when swap happened
                flag = true;
            }
        }
        // Checking if the flag is set or not
        if (!flag)
            break;
    }
}

void transactionDetails(const Transaction &t)
{
    cout << "ID: " << t.id << endl;
    cout << "Amount: " << t.amount << endl;
    cout << "Description: " << t.description << endl;
    cout << "Date: " << t.date << endl;
    cout << "Category: " << t.category << endl;
    cout << "Subcategory: " << t.subcategory << endl;
    cout << "--------------------------------- \n"
         << endl;
}
void displayTransaction(const vector<Transaction> &transactions)
{
    cout << "You selected Display all transactions." << endl;
    cout << "---------------------------------" << endl;

    if (transactions.empty())
    {
        cout << "No transactions to display." << endl;
        return;
    }
    cout << "Transactions Details:" << endl;

    // display all transactions
    for (int i = 0; i < transactions.size(); i++)
    {
        transactionDetails(transactions[i]);
    }
}
void sortTransactionsDetails(const vector<Transaction> &transactions)
{
    {
        if (transactions.empty())
        {
            cout << "No transactions to display." << endl;
            return;
        }
        vector<Transaction> sortedTransactions = transactions;
        int sortChoice;
        do
        {
            cout << "\n -------- -------- \n";
            cout << "Sorting options: \n 1. By ID \n 2. Amount";
            cout << "\n 3. Date" << endl;
            cout << "Enter your choice for sorting (4 to go back to main menu): ";
            while (true)
            {
                try
                {
                    sortChoice = inputValidation();
                    break; // exit loop if input is valid
                }
                catch (InputException &e)
                {
                    cout << e.what() << endl;
                    cout << "Please enter your choice again: ";
                }
            }
            switch (sortChoice)
            {
            case 1:
                cout << "--------------------------------" << endl;
                cout << "By ID." << endl;
                for (int i = 0; i < sortedTransactions.size(); i++)
                {
                    transactionDetails(sortedTransactions[i]);
                }
                break;

            case 2:
                cout << "\nYou selected Sort by Amount. \n"
                     << endl;
                bubbleSortForAmount(sortedTransactions);
                for (int i = 0; i < sortedTransactions.size(); i++)
                {
                    transactionDetails(sortedTransactions[i]);
                }
                break;
            case 3:
                cout << "\nYou selected Sort by Date. \n"
                     << endl;
                bubbleSortForDate(sortedTransactions);
                for (int i = 0; i < sortedTransactions.size(); i++)
                {
                    transactionDetails(sortedTransactions[i]);
                }
                break;

            case 4:
                cout << "Going back to main menu." << endl;
                break;

            default:
                cout << "Invalid choice. Please try again." << endl;
                cout << "---------------------------------" << endl;
                break;
            }
        } while (sortChoice != 4);
    }
}

// search function
void searchTransactions(const vector<Transaction> &transactions)
{
    cout << "\n-- Search Transactions --" << endl;
    cout << "---------------------------------" << endl;
    if (transactions.empty())
    {
        cout << "No transactions available to search." << endl;
        return;
    }
    int searchChoice = 0;
    do
    {
        cout << "Search by: \n 1. ID  \n 2. Amount \n 3. Category (income/expense) \n 4. Back to main menu" << endl;
        cout << "Enter your search choice: ";
        while (true)
        {
            try
            {
                searchChoice = inputValidation();
                break; // exit loop if input is valid
            }
            catch (InputException &e)
            {
                cout << e.what() << endl;
                cout << "Please enter your choice again: ";
            }
        }
        if (searchChoice == 1)
        {
            int idToSearch;
            cout << "Enter the Transaction ID to search: ";
            while (true)
            {
                try
                {
                    idToSearch = inputValidation();
                    break; // exit loop if input is valid
                }
                catch (InputException &e)
                {
                    cout << e.what() << endl;
                    cout << "Please enter the ID again: ";
                }
            }
            bool found = false;
            for (int i = 0; i < transactions.size(); i++)
            {
                if (transactions[i].id == idToSearch)
                {
                    transactionDetails(transactions[i]);
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                cout << "\n-----------------------\n";
                cout << "Transaction with ID " << idToSearch << " not found." << endl;
            }
        }
        else if (searchChoice == 2)
        {
            double amountToSearch;
            cout << "Enter the Transaction Amount to search: ";
            while (true)
            {
                try
                {
                    amountToSearch = inputValidationDouble();
                    break; // exit loop if input is valid
                }
                catch (InputException &e)
                {
                    cout << e.what() << endl;
                    cout << "Please enter the amount again: ";
                }
            }
            bool found = false;
            for (int i = 0; i < transactions.size(); i++)
            {
                if (transactions[i].amount == amountToSearch)
                {
                    transactionDetails(transactions[i]);
                    found = true;
                }
            }
            if (!found)
            {
                cout << "\n-----------------------\n";
                cout << "Transaction with Amount " << amountToSearch << " not found." << endl;
            }
        }
        else if (searchChoice == 3)
        {
            string categoryToSearch;
            cout << "Enter the Category (income/expense) to search: ";
            cin >> categoryToSearch;
            while (categoryToSearch != "income" && categoryToSearch != "expense")
            {
                cout << "Invalid category. Enter income/expense: ";
                cin >> categoryToSearch;
            }
            bool found = false;
            for (int i = 0; i < transactions.size(); i++)
            {
                if (transactions[i].category == categoryToSearch)
                {
                    transactionDetails(transactions[i]);
                    found = true;
                }
            }
            if (!found)
            {
                cout << "\n-----------------------\n";
                cout << "No transactions found in category " << categoryToSearch << "." << endl;
            }
        }
        else if (searchChoice == 4)
        {
            cout << "Going back to main menu." << endl;
        }
        else
        {
            cout << "Invalid choice. Please try again." << endl;
            cout << "---------------------------------" << endl;
        }
    } while (searchChoice != 4);
}

void saveTransaction(const vector<Transaction> &transactions, const string &filename)
{
    ofstream out(filename);
    if (!out)
    {
        cout << "Error opening file for writing.\n";
        return;
    }
    for (const auto &t : transactions)
    {
        out << t.id << "\n"
            << t.amount << "\n"
            << t.description << "\n"
            << t.date << "\n"
            << t.category << "\n"
            << t.subcategory << "\n";

        if (out.fail())
        {
            cout << "Error writing to file.\n";
            out.close();
            return;
        }
    }
    out.close();
    cout << "Customers saved to file.\n";
}

void loadCustomers(vector<Transaction> &transactions, const string &filename)
{
    ifstream in(filename);
    if (!in)
    {
        cout << "Error opening file for reading.6\n";
        return;
    }
    transactions.clear();
    while (in.peek() != EOF)
    {
        if (!in)
            break;
        Transaction t = Transaction::readFromFile(in);

        if (t.id == -1)
            continue;
        transactions.push_back(t);
    }
    in.close();
    if (!transactions.empty())
        Transaction::nextID = transactions.back().id + 1;
    cout << "Transactions loaded from file.\n";
}
// Main function
int main()
{
    vector<Transaction> transactions;
    int choice = 0;
    do
    {
        displayMenu();
        while (true)
        {
            try
            {
                choice = inputValidation();
                break; // exit loop if input is valid
            }
            catch (InputException &e)
            {
                cout << e.what() << endl;
                cout << "Please enter your choice again: ";
            }
        }
        switch (choice)
        {
        case 1:
        {
            Transaction newTransaction = addTransaction();
            transactions.push_back(newTransaction);
            cout << "Transaction added successfully." << endl;
        }
        break;
        case 2:
        {
            cout << "You selected Delete an existing transaction." << endl;
            cout << "---------------------------------" << endl;
            cout << "Delete by: \n 1. ID  \n 2. Amount \n 3. Go back to main menu" << endl;
            int deleteChoice;
            cout << "Enter your choice for deletion: ";
            while (true)
            {
                try
                {
                    deleteChoice = inputValidation();
                    break; // exit loop if input is valid
                }
                catch (InputException &e)
                {
                    cout << e.what() << endl;
                    cout << "Please enter your choice again: ";
                }
            }
            do
            {
                if (deleteChoice == 1)
                {
                    deleteTransactionbyID(transactions);
                    break;
                }
                else if (deleteChoice == 2)
                {
                    deleteTransactionbyAmount(transactions);
                    break;
                }
                else if (deleteChoice == 3)
                {
                    break;
                }
            } while (deleteChoice != 3);
            break;
        }

        case 3:
            displayTransaction(transactions);
            break;
        case 4:
            sortTransactionsDetails(transactions);
            break;
        case 5:
            searchTransactions(transactions);
            break;
        case 6:
            cout << "You selected Save all transactions to file." << endl;
            saveTransaction(transactions, "transactions1.txt");
            cout << "---------------------------------" << endl;
            break;

        case 7:
            cout << "You selected Load transactions from file." << endl;
            loadCustomers(transactions, "transactions1.txt");
            cout << "---------------------------------" << endl;
            break;

        case 8:
            cout << "Exiting the program." << endl;
            break;

        default:
            cout << "Invalid choice. Please try again." << endl;
            cout << "---------------------------------" << endl;
        }
    } while (choice != 8);

    return 0;
}
