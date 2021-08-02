//***********************
// Name: Sanaz Dehghannayyeri
//***********************



#include <iostream>
#include <string>
#include <occi.h>

using oracle::occi::Environment;
using oracle::occi::Connection;
using namespace oracle::occi;
using namespace std;

struct ShoppingCart
{
	int productID;
	double price;
	int quantity;
};


//The mainMenu() function returns an integer value which is the selected option by the user from the menu. This function displays the following menu options:
//1)	Login
//0)	Exit

int mainMenu()
{


	int choose = 0;
	do
	{
		//If the user chooses option 1, ask the user to enter customer ID to login. To see if the customer with the entered ID exists, call the Oracle stored procedure find_customer(). IF the value of the output parameter in the procedure is 1, let the customer to continue. If the value of the output parameter found is 0, call the mainMenu() functions again and asks the customer to login again. Continue this process until the user chooses the option 0 to exit or enters a valid customer ID.
		cout << "******************** Main Menu ********************" << endl;
		cout << "1)      Login" << endl;
		cout << "0)      Exit" << endl;

		cout << "Enter an choose (0-1): ";
		cin >> choose;

		while (choose != 0 && choose != 1)
		{
			cout << "******************** Main Menu ********************" << endl;
			cout << "1)      Login" << endl;
			cout << "0)      Exit" << endl;
			cout << "You entered a wrong value. Enter an choose (0-1): ";
			cin >> choose;
		}
		
	} while (choose == 0 && choose == 1);

	return choose;
}



//int customerLogin(Connection* conn, int customerId);This function receives an integer value as a customer ID and checks if the customer does exist in the database.
int customerLogin(Connection* connection, int customer_Id)
{

	int cust_ID;

	Statement* st = connection->createStatement();
	//This function returns 1 if the customer exists. If the customer does not exists, this function returns 0 and the main menu is displayed.To validate the customer ID call the find_customer() stored procedure in this function.
	st->setSQL("BEGIN find_customer(:1, :2); END;");
	st->setInt(1, customer_Id);
	st->registerOutParam(2, Type::OCCIINT, sizeof(cust_ID));
	st->executeUpdate();

	cust_ID = st->getInt(2);
	connection->terminateStatement(st);

	return cust_ID;
}
//This function receives an OCCI pointer (a reference variable to an Oracle database) and an integer value as the product ID.
double findProduct(Connection* connection, int product_ID)
{

	double price;
	Statement* st = connection->createStatement();
	//This functions calls the find_product() Oracle stored procedure. The procedure receives the product ID and returns the price. If the price is 0, the product ID is not valid (does not exist). If the price is a non-zero value, it means the product ID is valid.
	st->setSQL("BEGIN find_product(:1, :2); END;");
	st->setInt(1, product_ID);
	st->registerOutParam(2, Type::OCCIDOUBLE, sizeof(price));
	st->executeUpdate();

	price = st->getDouble(2);
	connection->terminateStatement(st);

	if (price > 0)
	{
		return price;
	}

	else
	{
		return 0;
	}
}




//If the customerLogin() functions return 1 (The customer ID exists), call this function. 
//This function receives an OCCI pointer(a reference variable to an Oracle database) and an array of type ShoppingCart.
int addToCart(Connection* connection, struct ShoppingCart Shop_cart[])
{


	cout << "-------------- Add Products to Cart --------------" << endl;
	int MaxNum = 0;
	while (MaxNum < 5)//The customer can purchase up to five items in one order. Write a loop to prompt the user to enter product IDs for the maximum of five products.
	{

		ShoppingCart ShopList;

		int productID;
		int product_Num;
		int Option;
		
		do
		{
			cout << "Enter the product ID: ";
			cin >> productID;

			
//When the user enters the product ID in the addToCart() function, call the findProduct() functions to check if the product ID exists. 
			if (findProduct(connection, productID) == 0)
			{
				cout << "The product does not exist. Try again..." << endl;
			}
//IF the product exists, the function findProduct() returns the product’s price. 
		} while (findProduct(connection, productID) == 0);
		//Display the product’s price to the user and ask the user to enter the quantity.
		cout << "Product Price: " << findProduct(connection, productID) << endl;
		cout << "Enter the product Quantity: ";
		cin >> product_Num;

		ShopList.productID = productID;
		ShopList.price = findProduct(connection, productID);
		ShopList.quantity = product_Num;
		Shop_cart[MaxNum] = ShopList;

		if (MaxNum == 4)
			return MaxNum + 1;
		else
		{//If the user enters a valid product ID, display the following message and let the user to enter another product ID.
			do
			{//"Enter 1 to add more products or 0 to checkout: "
				cout << "Enter 1 to add more products or 0 to check out: ";
				cin >> Option;
			} while (Option != 0 && Option != 1);
		}
//If the user enters 0, the function addToCart(), returns the number of products (items) entered by the user.
		if (Option == 0)
		{
			return MaxNum + 1;
		}

		MaxNum++;
	}
}




//This function receives an array of type ShoppingCart and the number of ordered items (products). It display the product ID, price, and quantity for products stored in the cart array.
void displayProducts(struct ShoppingCart ShopCart[], int pr_Count)
{

	if (pr_Count > 0)
	{

		double total = 0.00;

		cout << "------- Ordered Products ---------" << endl;
		for (int i = 0; i < pr_Count; i++)
		{
			cout << "---Item " << i + 1 << endl;
			cout << "Product ID: " << ShopCart[i].productID << endl;
			cout << "Price: " << ShopCart[i].price << endl;
			cout << "Quantity: " << ShopCart[i].quantity << endl;
			//After displaying the products’ information (product ID, price, and quantity), display the total order amount. To calculate the total order amount, first multiply the quantity and the price to calculate the total amount for each product. 
			total += ShopCart[i].price * ShopCart[i].quantity;
		}

		cout << "----------------------------------\nTotal: " << total << endl;
	}
}




//This function receives an OCCI pointer (a reference variable to an Oracle database), an array of type ShoppingCart, an integer value as the customer ID, and an integer value as the number of ordered items (products).
int checkout(Connection* connection, struct ShoppingCart ShopCart[], int customerId, int pr_Count)
{

	char data;

	do
	{
		cout << "Would you like to checkout? (Y/y or N/n)  ";
		cin >> data;

		if (data != 'Y' && data != 'y' && data != 'N' && data != 'n')
			//If the user enters any values except “Y/y” and “N/n”, display a proper message and ask the user to enter the value again.
			cout << "Wrong input. Try again..." << endl;
	} while (data != 'Y' && data != 'y' && data != 'N' && data != 'n');
	//If the user enters “N/n”, the function checkout() terminates and returns 0.
	if (data == 'N' || data == 'n')
	{
		cout << "The order is cancelled." << endl;
		return 0;
	}
	else
	{//If the user enters “Y/y”, the Oracle stored procedure add_order() is called. This procedure will add a row in the orders table with a new order ID.This stored procedure returns an order ID, which will be used to store ordered items in the table order_items.

		int orderID;
//For all products in the array cart (productCount is the number of products stored in the array cart), call the stored procedure add_order_item() and pass the corresponding values to this stored procedure. 
		Statement* stmt = connection->createStatement();
		stmt->setSQL("BEGIN add_order(:1, :2); END;");
		stmt->setInt(1, customerId);
		stmt->registerOutParam(2, Type::OCCIINT, sizeof(orderID));
		stmt->executeUpdate();
		orderID = stmt->getInt(2);

		int product = 0;
		while (product < pr_Count)
		{
			stmt->setSQL("BEGIN add_order_item(:1, :2, :3, :4, :5); END;");
			stmt->setInt(1, orderID);
			stmt->setInt(2, product + 1);
			stmt->setInt(3, ShopCart[product].productID);
			stmt->setInt(4, ShopCart[product].quantity);
			stmt->setDouble(5, ShopCart[product].price);
			stmt->executeUpdate();
			product++;
		}

		cout << "The order is successfully completed." << endl;
		connection->terminateStatement(stmt);

		return 1;
	}
}




//int mainMenu();
//The mainMenu() function returns an integer value which is the selected option by the user from the menu.
int main()
{

	// declare the environmentand the connection variables.
	Environment* env = nullptr;
	Connection* connection = nullptr;


	//Define and initialize the variable to store the username, password, and the host address.
	string user = "dbs311_203h06";
	string pass = "19294127";
	string constr = "myoracle12c.senecacollege.ca:1521/oracle12c";



	//handle any errors 
	try
	{
		env = Environment::createEnvironment(Environment::DEFAULT);
		connection = env->createConnection(user, pass, constr);
		cout << "Connection is successful!" << endl;


		int data;
		int cust_id;
		do
		{
			data = mainMenu();

			switch (data)
			{

			case 1://If the user chooses option 1, ask the user to enter customer ID to login.call the Oracle stored procedure find_customer(). 
				
				cout << "Enter the customer ID: ";
				cin >> cust_id;
				//If the value of the output parameter found is 0, call the mainMenu() functions again and asks the customer to login again. 
				if (customerLogin(connection, cust_id) == 0)
				{
					//Call int customerLogin(Connection* conn, int customerId); function in main
					cout << "The customer does not exist." << endl;
				}
				else
				{
					ShoppingCart ShopCart[5];
					int count = addToCart(connection, ShopCart);
					displayProducts(ShopCart, count);
					checkout(connection, ShopCart, cust_id, count);
				}
				break;

			}
//Continue this process until the user chooses the option 0 to exit or enters a valid customer ID.

		} while (data != 0);

		cout << "Good bye!..." << endl;

		//terminate and close the connection and the environment, when your program terminates.
		env->terminateConnection(connection);
		Environment::terminateEnvironment(env);
	}
	catch (SQLException & sqlExcp)
	{
		cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
	}
	return 0;
}
