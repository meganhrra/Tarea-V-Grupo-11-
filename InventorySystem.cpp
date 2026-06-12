#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

/*
 * Product class represents a product in the inventory.
 */
class Product
{
private:
    string name;
    double price;
    int quantity;

public:
    /*
     * Constructor to initialize product attributes.
     */
    Product(string name, double price, int quantity)
    {
        this->name = name;
        this->price = price;
        this->quantity = quantity;
    }

    /*
     * Getter for product name.
     */
    string getName() const
    {
        return name;
    }

    /*
     * Getter for product price.
     */
    double getPrice() const
    {
        return price;
    }

    /*
     * Getter for product quantity.
     */
    int getQuantity() const
    {
        return quantity;
    }

    /*
     * Displays product information.
     */
    void display() const
    {
        cout << "Name: " << name
             << " | Price: $" << price
             << " | Quantity: " << quantity << endl;
    }
};

/*
 * Allows the user to add a new product to the inventory.
 */
void addProduct(vector<Product>& products)
{
    string name;
    double price;
    int quantity;

    cout << "\n=== Add New Product ===" << endl;

    cout << "Enter product name: ";
    cin >> name;

    cout << "Enter product price: ";
    cin >> price;

    cout << "Enter product quantity: ";
    cin >> quantity;

    products.push_back(Product(name, price, quantity));

    cout << "\nProduct added successfully!" << endl;
}

/*
 * Displays all products in the inventory.
 */
void displayInventory(const vector<Product>& products)
{
    if (products.empty())
    {
        cout << "\nInventory is empty." << endl;
        return;
    }

    cout << "\n===== INVENTORY =====" << endl;

    for (const Product& product : products)
    {
        product.display();
    }
}

/*
 * Sorts products by price in ascending order.
 */
void sortProductsByPrice(vector<Product>& products)
{
    sort(products.begin(), products.end(),
         [](const Product& p1, const Product& p2)
         {
             return p1.getPrice() < p2.getPrice();
         });

    cout << "\nProducts sorted by price successfully." << endl;
}

/*
 * Searches for a product by name.
 * Returns the index if found, otherwise -1.
 */
int searchProductByName(const vector<Product>& products, const string& productName)
{
    for (size_t i = 0; i < products.size(); i++)
    {
        if (products[i].getName() == productName)
        {
            return static_cast<int>(i);
        }
    }

    return -1;
}

/*
 * Recursively calculates the total number of units available.
 */
int calculateTotalUnitsRecursive(const vector<Product>& products, int index)
{
    // Base case: reached the end of the vector
    if (index == products.size())
    {
        return 0;
    }

    // Recursive case
    return products[index].getQuantity() +
           calculateTotalUnitsRecursive(products, index + 1);
}

/*
 * Main program.
 */
int main()
{
    // Initial inventory
    vector<Product> products =
    {
        Product("Laptop", 1200.00, 5),
        Product("Mouse", 25.50, 20),
        Product("Keyboard", 45.99, 15),
        Product("Monitor", 300.00, 8)
    };

    int option;

    do
    {
        cout << "\n=================================" << endl;
        cout << "       INVENTORY SYSTEM" << endl;
        cout << "=================================" << endl;
        cout << "1. Add Product" << endl;
        cout << "2. Display Inventory" << endl;
        cout << "3. Sort Products by Price" << endl;
        cout << "4. Search Product by Name" << endl;
        cout << "5. Calculate Total Units" << endl;
        cout << "0. Exit" << endl;

        cout << "\nSelect an option: ";
        cin >> option;

        switch (option)
        {
            case 1:
            {
                addProduct(products);
                break;
            }

            case 2:
            {
                displayInventory(products);
                break;
            }

            case 3:
            {
                sortProductsByPrice(products);
                break;
            }

            case 4:
            {
                string productName;

                cout << "\nEnter product name to search: ";
                cin >> productName;

                int index = searchProductByName(products, productName);

                if (index != -1)
                {
                    cout << "\nProduct found:" << endl;
                    products[index].display();
                }
                else
                {
                    cout << "\nProduct not found." << endl;
                }

                break;
            }

            case 5:
            {
                int totalUnits =
                    calculateTotalUnitsRecursive(products, 0);

                cout << "\nTotal available units in inventory: "
                     << totalUnits << endl;

                break;
            }

            case 0:
            {
                cout << "\nExiting program..." << endl;
                break;
            }

            default:
            {
                cout << "\nInvalid option. Please try again." << endl;
            }
        }

    } while (option != 0);

    return 0;
}