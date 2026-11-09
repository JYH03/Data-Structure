#include "Manager.h"

namespace
{
vector<string> splitCommand(const string &line)
{
    vector<string> tokens;
    string token;
    istringstream input(line);
    while (getline(input, token, '\t'))
        tokens.push_back(token);
    if (!line.empty() && line.back() == '\t')
        tokens.push_back("");
    return tokens;
}

bool parseInt(const string &value, int &result)
{
    try
    {
        size_t position = 0;
        result = stoi(value, &position);
        return position == value.size();
    }
    catch (...)
    {
        return false;
    }
}
}

void Manager::run(const char *command)
{
    this->fin.open(command); // Receive commands from the input file specified by 'command'
    if (fin.is_open())
    {
        this->flog.open("log.txt");                 // Open log.txt file to record execution results, success or failure
        string line;
        while (getline(this->fin, line))            // Repeatedly read commands into 'line'
        {
            if (!line.empty() && line.back() == '\r')
                line.pop_back();

            vector<string> tokens = splitCommand(line);
            string order = tokens.empty() ? "" : tokens[0];

            if (order == "LOAD")
            {
                if (tokens.size() == 1)
                    LOAD();
                else
                    printErrorCode(100);
            }
            else if (order == "ADD_BP")
            {
                int deptNo, id, income;
                if (tokens.size() != 5 || tokens[1].empty() ||
                    !parseInt(tokens[2], deptNo) ||
                    !parseInt(tokens[3], id) ||
                    !parseInt(tokens[4], income))
                    printErrorCode(200);
                else
                    ADD_BP(tokens[1], tokens[2], tokens[3], tokens[4]);
            }
            else if (order == "SEARCH_BP")
            {
                if ((tokens.size() != 2 && tokens.size() != 3) ||
                    tokens[1].empty() ||
                    (tokens.size() == 3 && tokens[2].empty()))
                    printErrorCode(300);
                else if (tokens.size() == 2)
                    SEARCH_BP_NAME(tokens[1]);
                else
                    SEARCH_BP_RANGE(tokens[1], tokens[2]);
            }
            else if (order == "PRINT_BP")
            {
                if (tokens.size() == 1)
                    PRINT_BP();
                else
                    printErrorCode(400);
            }
            else if (order == "ADD_ST")
            {
                if (tokens.size() != 3 || tokens[2].empty())
                {
                    printErrorCode(500);
                }
                else if (tokens[1] == "dept_no")
                {
                    int deptNo;
                    if (parseInt(tokens[2], deptNo))
                        ADD_ST_DEPTNO(deptNo);
                    else
                        printErrorCode(500);
                }
                else if (tokens[1] == "name")
                {
                    ADD_ST_NAME(tokens[2]);
                }
                else
                {
                    printErrorCode(500);
                }
            }
            else if (order == "PRINT_ST")
            {
                int deptNo;
                if (tokens.size() != 2 || !parseInt(tokens[1], deptNo))
                    printErrorCode(600);
                else
                    PRINT_ST(deptNo);
            }
            else if (order == "DELETE")
            {
                if (tokens.size() == 1)
                    DELETE();
                else
                    printErrorCode(700);
            }
            else if (order == "EXIT")
            {
                if (tokens.size() != 1)
                {
                    printErrorCode(800);
                    continue;
                }
                printSuccessCode("EXIT");
                this->fin.close();  // Close input file stream
                this->flog.close(); // Close output file stream
                return;
            }
            else
            {
                printErrorCode(800); // Error for invalid command
            }
        }
    }
}

void Manager::LOAD() // Function to read employee information from employee.txt and insert into B+ tree
{
    // Open employee.txt
    this->f_employee.open("employee.txt");
    if (!this->f_employee.is_open()) // Error if file fails to open
    {
        printErrorCode(100);
        this->f_employee.close();
        return;
    }
    else
    {
        // Error if bptree is not empty
        if (this->bptree->getRoot())
        {
            printErrorCode(100);
            this->f_employee.close();
            return;
        }
        string tmp, name, Department_code, id_number, annual_income; // Temporary variables to store data
        EmployeeData *data;
        while (getline(this->f_employee, tmp)) // Read employee data line by line
        {
            data = new EmployeeData; // Create EmployeeData object
            this->ss.clear();
            this->ss.str(tmp);
            // Separate the data by '\t'.
            getline(ss, name, '\t');
            getline(ss, Department_code, '\t');
            getline(ss, id_number, '\t');
            getline(ss, annual_income, '\t');
            if (!(name.empty() || id_number.empty() || Department_code.empty() || annual_income.empty())) // If all data is valid
            {
                data->setData(name, stoi(Department_code), stoi(id_number), stoi(annual_income)); // Pass temporary data to EmployeeData object
                if (!this->bptree->Insert(data))                                                  // Insert object into bptree, error if duplicate
                {
                    printErrorCode(100);
                    this->f_employee.close();
                    return;
                }
            }
            else
            {
                // Error if data is invalid
                printErrorCode(100);
                this->f_employee.close();
                return;
            }
        }
    }
    printSuccessCode("LOAD"); // Print success message
    this->f_employee.close(); // Exit ifstream
}

void Manager::ADD_BP(string name, string dept_no, string id, string income) // Function to manually add employee data
{
    if (!(name.empty() || id.empty() || dept_no.empty() || income.empty())) // If all data is valid
    {
        EmployeeData *data = new EmployeeData;                     // Create new object to be inserted
        data->setData(name, stoi(dept_no), stoi(id), stoi(income)); // Pass temporary data to EmployeeData object
        if (this->bptree->Insert(data))                             // Insert object into bptree, if not duplicate
        {
            this->stree->Refresh(); // Reorder copied pointers if an existing income was updated
            this->flog << "========ADD_BP========" << endl; // Print success message
            this->flog << name << '/' << dept_no << '/' << id << '/' << income << endl;
            this->flog << "=======================\n\n";
        }
        else
        {
            delete data;
            printErrorCode(200);
        }
    }
    else
    {
        printErrorCode(200); // Error due to invalid input
        return;
    }
}

void Manager::SEARCH_BP_NAME(string name) // Function to search data in B+ tree using the given name as key
{
    if (!this->bptree->getRoot()) // Error if B+ tree is empty
    {
        printErrorCode(300);
        return;
    }
    BpTreeNode *ptr = this->bptree->searchDataNode(name); // Search the DataNode that contains data with the given name as key
    auto it = ptr->getDataMap()->find(name);              // Search for data matching 'name' inside the DataNode
    if (it != ptr->getDataMap()->end())                   // Iterator did not reach the end → key exists
    {
        this->flog << "========SEARCH_BP========" << endl;
        this->flog << name << '/' << it->second->getDeptNo() << '/' << it->second->getID() << '/' << it->second->getIncome() << endl; // Output the found data
        this->flog << "=======================\n\n";
    }
    else
    {   
        printErrorCode(300); // Iterator reached the end → key does not exist, so error
    }
}

void Manager::SEARCH_BP_RANGE(string start, string end) // Function to search all data in B+ tree between two given keys
{
    if (!this->bptree->getRoot()) // Error if B+ tree is empty
    {
        printErrorCode(300);
        return;
    }
    this->printer.clear();
    this->printer = this->bptree->printData(start, end); // Print all key data between the two inputs, internally uses searchRange
    if (this->printer.empty())                           // Error if no data found
    {
        printErrorCode(300);
        return;
    }
    this->flog << "========SEARCH_BP========" << endl;
    for (unsigned int i = 0; i < this->printer.size(); i++)
    {
        this->flog << printer[i] << endl; // Output all found data
    }
    this->flog << "=======================\n\n";
}

void Manager::ADD_ST_DEPTNO(int dept_no) // Function to find data matching the given department code and insert into selection tree
{
    if (dept_no % 100 != 0 || (dept_no < 100 || dept_no > 800))
    {
        printErrorCode(500);
        return;
    }
    vector<EmployeeData *> Data = this->bptree->searchDeptNo(dept_no); // Linearly search DataNodes in B+ tree for matching department code
    if (Data.empty())                                                  // Error if no matching data found
    {
        printErrorCode(500);
    }
    else
    {
        for (unsigned int i = 0; i < Data.size(); i++)
        {
            if (!this->stree->Insert(Data[i])) // Insert matching data into selection tree
            {
                printErrorCode(500); // Error if department code is not one of 100, 200, ..., 800
            }
        }
        printSuccessCode("ADD_ST"); // Print success message
    }
}

void Manager::ADD_ST_NAME(string name) // Function to find data in B+ tree matching the given name (key) and insert into selection tree
{
    if (!this->bptree->getRoot())
    {
        printErrorCode(500);
        return;
    }
    BpTreeNode *ptr = this->bptree->searchDataNode(name); // Search from root through index nodes for DataNode matching the name key
    if (ptr->getDataMap()->count(name))                   // If DataNode contains data with the given name key
    {
        auto it = ptr->getDataMap()->find(name); // Find the position
        if (!this->stree->Insert(it->second))    // Insert data into selection tree
        {
            printErrorCode(500); //
            return;
        }
        printSuccessCode("ADD_ST"); // Print success message
    }
    else
    {
        printErrorCode(500); // Error if data does not exist
        return;
    }
}

void Manager::PRINT_BP() // Function to print all data in B+ tree
{
    if (!this->bptree->getRoot()) // Error if B+ tree is empty
    {
        printErrorCode(400);
        return;
    }
    this->printer.clear();
    this->printer = this->bptree->printData(); // Linearly search and store all data in B+ tree, internally uses searchAll in printData
    if (this->printer.empty())
    {
        printErrorCode(400);
        return;
    }
    this->flog << "========PRINT_BP========" << endl;
    for (unsigned int i = 0; i < this->printer.size(); i++)
    {
        this->flog << printer[i] << endl; // Print all data
    }
    this->flog << "=======================\n\n";
}

void Manager::PRINT_ST(int dept_no) // Function to print all data in selection tree matching the given department code
{
    if (!this->stree->getRoot()->IsEmpty()) // If selection tree is not empty
    {
        this->printer.clear();
        this->printer = this->stree->printEmployeeData(dept_no); // Search for data in selection tree matching the department code
        if (this->printer.empty())                               // Error if no data found or invalid department code (returns immediately)
        {
            printErrorCode(600);
            return;
        }
        this->flog << "========PRINT_ST========" << endl;
        for (unsigned int i = 0; i < printer.size(); i++)
        {
            flog << this->printer[i] << endl; // Print data matching the department code
        }
        this->flog << "=======================\n\n";
    }
    else
    {
        printErrorCode(600); // Error if selection tree is empty
    }
}

void Manager::DELETE() // Function to delete the root node (top data) of selection tree
{
    if (this->stree->Delete()) // If the data structure is not empty
    {
        printSuccessCode("DELETE");
    }
    else
    {
        printErrorCode(700); // Error if empty
    }
}

void Manager::printErrorCode(int n) // Function to print error messages
{
    flog << "========ERROR========\n";
    flog << n << "\n";
    flog << "=====================\n\n";
}

void Manager::printSuccessCode(string success) // Function to print success messages
{
    flog << "========" << success << "========\n";
    flog << "Success" << "\n";
    flog << "====================\n\n";
}
