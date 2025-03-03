#include <iostream>
#include <fstream>
#include <stack>
#include <sstream>
#include <vector>

class LineReader 
{
public:
    friend std::istream& operator>>(std::istream& is, LineReader& lr)
    {
        if (!std::getline(is, lr.line_))
        {
            lr.line_.clear();
        }
        return is;
    }

    const std::string& getLine() const
    {
        return line_;
    }

    bool empty() const
    {
        return line_.empty();
    }

private:
    std::string line_;
};

class LStringStack 
{
public:
    void pushLine(const std::string& text)
    {
        m_stack.push(text);
    }

    std::string popLine()
    {
        if (m_stack.empty())
        {
            return std::string();
        }
        std::string top = m_stack.top();
        m_stack.pop();
        return top;
    }

    bool isEmpty() const
    {
        return m_stack.empty();
    }

    void clearStack()
    {
        while (!m_stack.empty())
        {
            m_stack.pop();
        }
    }

    void printStack() const
    {
        std::stack<std::string> temp = m_stack;
        std::cout << "Stack (top -> bottom):\n\n";
        while (!temp.empty())
        {
            std::cout << temp.top() << "\n";
            temp.pop();
        }
    }

    void reverseStack()
    {
        std::stack<std::string> reversed;
        while (!m_stack.empty())
        {
            reversed.push(m_stack.top());
            m_stack.pop();
        }
        m_stack.swap(reversed);
    }

    // Renumber lines in the stack from top to bottom
    void renumberStack()
    {
        std::vector<std::string> lines;
        while (!m_stack.empty())
        {
            lines.push_back(m_stack.top());
            m_stack.pop();
        }

        std::reverse(lines.begin(), lines.end());

        for (size_t i = 0; i < lines.size(); i++)
        {
            std::ostringstream oss;
            oss << (i + 1) << ": " << lines[i];
            m_stack.push(oss.str());
        }
    }

protected:
    std::stack<std::string> m_stack;  // Internal storage
};

class FileStack : public LStringStack {
public:
    // Reads entire lines from a file into the stack, 
    // pushing each new line on top in the order they appear in the file
    bool loadFromFile(const std::string& filename)
    {
        std::ifstream ifs(filename);
        if (!ifs)
        {
            std::cerr << "Couldnt open file: " << filename << std::endl;
            return false;
        }

        std::istream_iterator<LineReader> start(ifs), end;
        while (start != end)
        {
            if (!start->empty())
            {
                pushLine(start->getLine());
            }
            start++;
        }
        return true;
    }

    // Writes the stack contents to a file from top to bottom
    bool saveToFile(const std::string& filename) const
    {
        std::ofstream ofs(filename);
        if (!ofs.is_open())
        {
            std::cerr << "Couldnt open file for writing: " << filename << std::endl;
            return false;
        }

        std::stack<std::string> temp = m_stack;
        std::vector<std::string> lines;
        while (!temp.empty())
        {
            lines.push_back(temp.top());
            temp.pop();
        }
        std::ostream_iterator<std::string> outIter(ofs, "\n");
        std::copy(lines.begin(), lines.end(), outIter);

        return true;
    }

    // The renumbering is done in ascending order from top to bottom of the new stack.
    bool makeRenumberedCopy(const std::string& inFile, const std::string& outFile)
    {
        clearStack();
        if (!loadFromFile(inFile))
        {
            return false;
        }
        renumberStack();
        if (!saveToFile(outFile))
        {
            return false;
        }
        return true;
    }
};

int main()
{
    FileStack myFileStack;

    while (true)
    {
        std::cout << "\n=== MENU ===\n"
            << "1. Read & print file (using line iterators, no stack)\n"
            << "2. Load file into stack & print stack\n"
            << "3. Reverse the current stack\n"
            << "4. Renumber the current stack\n"
            << "5. Create renumbered copy of a file\n"
            << "6. Clear the current stack\n"
            << "7. Exit\n"
            << "Select an option: ";

        int choice = 0;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == 1)
        {
            std::cout << "Enter file name: ";
            std::string filename;
            std::getline(std::cin, filename);

            // Read and print lines from file, no stack involvement
            std::ifstream ifs(filename);
            if (!ifs)
            {
                std::cerr << "Could not open file: " << filename << "\n";
                continue;
            }
            std::istream_iterator<LineReader> start(ifs), end;
            std::cout << "\n--- File contents ---\n";
            while (start != end)
            {
                if (!start->empty())
                {
                    std::cout << start->getLine() << "\n";
                }
                start++;
            }
            std::cout << "--- End of file ---\n";
        }
        else if (choice == 2)
        {
            std::cout << "Enter file name to load: ";
            std::string filename;
            std::getline(std::cin, filename);

            myFileStack.clearStack();
            if (!myFileStack.loadFromFile(filename))
            {
                continue;
            }
            std::cout << "File loaded into stack. Now printing stack:\n";
            myFileStack.printStack();
        }
        else if (choice == 3)
        {
            std::cout << "Reversing current stack...\n";
            myFileStack.reverseStack();
            std::cout << "Now printing reversed stack:\n";
            myFileStack.printStack();
        }
        else if (choice == 4)
        {
            std::cout << "Renumbering current stack...\n";
            myFileStack.renumberStack();
            myFileStack.printStack();
        }
        else if (choice == 5)
        {
            std::cout << "Enter input file name: ";
            std::string inFile;
            std::getline(std::cin, inFile);
            std::cout << "Enter output file name (renumbered copy): ";
            std::string outFile;
            std::getline(std::cin, outFile);

            if (myFileStack.makeRenumberedCopy(inFile, outFile))
            {
                std::cout << "Renumbered copy created as '" << outFile << "'\n";
            }
        }
        else if (choice == 6)
        {
            std::cout << "Clearing current stack...\n";
            myFileStack.clearStack();
        }
        else if (choice == 7)
        {
            std::cout << "Exiting...\n";
            break;
        }
        else
        {
            std::cout << "Invalid choice!\n";
        }
    }

    return 0;
}