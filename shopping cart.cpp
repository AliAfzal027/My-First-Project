#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
using namespace std;

const int MAX_PRODUCTS = 100;
const int MAX_CART = 50;

class Store;
class CartItem;
class Customer;
class User
{
protected:
    string name;
public:
    virtual void panel() = 0;
    void setName()
	{
        cout<<"Enter your name: ";
        cin.ignore();
        getline(cin,name);
    }
    string getName()
	{
        return name;
    }
};

class Product
{
    int id;
    string name;
    string category;
    double price;
    int stock;
	public:
	friend class Customer;
	friend class Store;
	friend class CartItem;
    void input()
	try{
        cout<<"Enter Name: "; 
        cin.ignore();
		getline(cin, name);
        cout<<"Enter Category: "; 
		getline(cin, category);
        cout<<"Enter Price: "; 
		cin>>price;
		 if(price<=0) 
		 throw "Price must be positive.";
        cout<<"Enter Stock: "; 
		cin>>stock;
		 if(stock<0)
		 throw "Stock cannot be negative.";
    }
    catch (const char* e)
	{
            cout<<"execption accour: "<<e<<endl;
            input();
    }

    void display()
	{
        cout << id << " | " << name << " | " << category << " | Rs." << price << " | Stock: " << stock << endl;
    }
};

class Store
{
protected:
    Product products[MAX_PRODUCTS];
    int count;

public:
    Store() { count=0; }

    void loadProducts()
	{
        ifstream fin("products.txt");
        if(!fin.is_open())
        {
        	cout<<"File is not open!"<<endl;
		}
		else
        {
        	count = 0;
        string line;
        while (count < MAX_PRODUCTS && !fin.eof()) 
		{
            string idStr, name, category, priceStr, stockStr;
            getline(fin,idStr);
            getline(fin,name);
            getline(fin,category);
            getline(fin,priceStr);
            getline(fin,stockStr);

            Product p;
            p.name =name;
            p.category=category;
            stringstream(idStr)>>p.id;
        	stringstream(priceStr)>>p.price;
        	stringstream(stockStr)>>p.stock;

            products[count]=p;
            count++;
        }
        fin.close();
		}
    }

    void saveProducts()
	{
        ofstream fout("products.txt");
        if(!fout.is_open())
        {
        	cout<<"File is not open"<<endl;
		}
		else
        {
        	for(int i = 0; i < count; i++) {
            fout<<products[i].id<<endl<< products[i].name<<endl<<products[i].category
                 <<endl<< products[i].price<<endl<<products[i].stock <<endl;
        }
		}
        fout.close();
    }

    void showAllProducts()
	{
		if(count==0)
		{
			cout<<"/nInventry is empty!"<<endl;
		}
		else
        {
	        for (int i = 0; i < count; i++)
			{
	            products[i].display();
	        }
		}
    }

    void showByCategory(string cat)
	{
        for(int i=0;i<count;i++)
		{
            if (products[i].category == cat)
			{
                products[i].display();
            }
        }
    }

    Product* findById(int id)
	{
        for(int i=0;i<count;i++)
		{
            if(products[i].id == id) 
			return &products[i];
        }
        return NULL;
    }

    void addProduct()
	{
        if(count>=MAX_PRODUCTS)
		{
            cout<<"Product limit reached."<<endl;
            return;
        }
        while(true)
        {
        	Product p;
	        try
			{
	        	cout<<"\nEnter ID (Press 0 to go back): "; 
	       	    cin.ignore();
				cin>>p.id;
				if (p.id == 0)
				{
                	cout<<"Exiting product addition...\n";
                	return;
            	}
			    if(p.id<0)
			    throw "ID must be positive";
				for(int i=0; i<count; i++)
				{
	            	if (products[i].id==p.id)
					{
	                	throw "This product ID already exists! Choose a unique ID.";
	           		}
	           	}
	        	p.input();
	        	products[count++] = p;
	        	saveProducts();
	        	cout<<"Product added successfully!"<<endl;
			}
			catch (const char* e)
			{
	        	cout<<"Error: "<<e<<endl;
	        	addProduct();
	   		}
		}
        
    }

    void updateProduct()
	{
       while(true)
       {
       	  int id;
        cout<<"\nEnter Product ID to update (press 0 to go back): ";
        cin>>id;
        if(id==0)
        {
        	cout<<"Exiting product updation!"<<endl;
        	return;
		}
        Product* p = findById(id);
        if(p !=NULL)
		{
            cin.ignore();
            p->input();
            saveProducts();
        } else
		{
            cout << "Product not found.\n";
        }
	   }
    }

    void deleteProduct()
	{
    	int id;
	    cout<<"\nEnter Product ID to delete: ";
	    cin>>id;
	    for(int i=0; i<count; i++)
		{
	    	if(products[i].id == id)
			{
	        for(int j = i; j<count-1; j++)
				{
	                products[j] = products[j + 1];
	            }
	            count--;
	            saveProducts();
	            cout<<"Product deleted.\n";
	            return;
	            }
	        }
	        cout<<"Product not found.\n";
    }
    void displayCategories()
{
    cout<<"Product Categories:"<<endl;
    int printed = 0; 

    for(int i=0; i<count; i++)
    {
        int dup = 0;
        for (int j = 0; j < i; j++)
        {
            if (products[i].category == products[j].category)
            {
                dup = 1;
                break;
            }
        }
        if (dup == 0)
        {
            cout<<products[i].category<<endl;
            printed++;
        }
    }

    if (printed == 0)
        cout << "No categories found.\n";
}
};

class CartItem
{
public:
    Product product;
    int quantity;
};

class Admin:public User
{
public:
    void panel()
	{
        Store store;
        store.loadProducts();

        string pin;
        cout<< "Enter Admin PIN: ";
        cin>>pin;
        if(pin!="123456")
		{
            cout<<"Access Denied.\n";
            return;
        }

        int choice;
        do {
            cout<<"\n--- Admin Panel ---"<<endl;
            cout<<"1. Add Product"<<endl;
			cout<<"2. Update Product"<<endl;
			cout<<"3. Delete Product"<<endl;
            cout<<"4. View Products"<<endl;
			cout<<"5. View Sales History"<<endl;
			cout<<"6. Back"<<endl;
            cout<<"Enter choice: ";
            cin>>choice;

            switch (choice) {
                case 1:
				store.addProduct();
				break;
                case 2:
				store.updateProduct();
				break;
                case 3:
				store.deleteProduct();
				break;
                case 4:
				store.showAllProducts();
				break;
                case 5: 
				{
					cout<<endl;
                    ifstream f("history.txt");
                    if(!f.is_open())
                    {
                    	cout<<"File is not opening"<<endl;
					}
                   else
                   {
                   	 string line;
                    while (getline(f, line)) 
					cout<<line<<endl;
                    f.close();
				   }
                }
				break;
                case 6:
				return;
                default:
				cout<<"Invalid choice."<<endl;
            }
        } while (true);
    }
};

class Customer:public User
{
public:
    void panel()
    {
        Store store;
        CartItem cart[MAX_CART];
        int cartCount=0;

        store.loadProducts();
        setName();

        int choice;
        do {
            cout<<"\n--- Customer Panel ---"<<endl;
            cout<<"1. View All Products"<<endl;
			cout<<"2. View By Category"<<endl;
			cout<<"3. Checkout"<<endl;
			cout<<"4. Back"<<endl;
            cout<<"Enter choice: ";
            cin>>choice;

            switch (choice)
			{
            	case 1:
                {
                    store.showAllProducts();
                    int id, qty;
                    cout<<"\nEnter Product ID (0 to stop): ";
                    cin>>id;
                    while(id != 0)
					{
                        Product* p = store.findById(id);
                        if(p != NULL && p->stock>0)
                        {
						  cout<<"Enter Quantity: ";
                          cin>>qty;
                            try {
                                if (qty<=0) 
								throw "Quantity must be positive.";
                                if(qty>p->stock)
								throw "Not enough stock.";

                                p->stock -= qty;
                                cart[cartCount].product = *p;
                                cart[cartCount].quantity = qty;
                                cartCount++;
                                store.saveProducts();
                            }
                            catch (const char* e)
							{
                                cout<<"Error: "<<e<<endl;
                            }
                        }
						else
						{
                            cout<<"Product not found.\n";
                        }
                        cout<<"\nEnter Product ID (0 to stop): ";
                        cin>>id;
                    }
                }
				break;

                case 2: 
				{
                    string category;
                    store.displayCategories();
                    cout<<"\nEnter Category: ";
                    cin.ignore();
                    getline(cin, category);
                    store.showByCategory(category);
                    
                    int id, qty;
                    cout<<"\nEnter Product ID (0 to stop): ";
                    cin>>id;
                    while(id != 0)
					{
                        Product* p = store.findById(id);
                        if(p != NULL && p->stock>0)
                        {
						  cout<<"Enter Quantity: ";
                          cin>>qty;
                            try {
                                if (qty<=0) 
								throw "Quantity must be positive.";
                                if(qty>p->stock)
								throw "Not enough stock.";

                                p->stock -= qty;
                                cart[cartCount].product = *p;
                                cart[cartCount].quantity = qty;
                                cartCount++;
                                store.saveProducts();
                            }
                            catch (const char* e)
							{
                                cout<<"Error: "<<e<<endl;
                            }
                        }
						else
						{
                            cout<<"Product not found.\n";
                        }
                        cout<<"\nEnter Product ID (0 to stop): ";
                        cin>>id;
                    }
                }
				break;

                case 3: 
				{
					cout<<endl;
                    double total = 0;
                    cout<<"\n--- Cart Summary ---"<<endl;
                    for(int i=0; i<cartCount; i++) 
					{
                        double sub=cart[i].product.price*cart[i].quantity;
                        cout<<cart[i].product.name<<" x"<<cart[i].quantity<<" = Rs."<<sub<<endl;
                        total+=sub;
                    }
                    cout<<"Total Bill = Rs."<<total<<endl;

                    ofstream f("history.txt", ios::app);
                    f<<"Buyer: "<<getName()<<endl;
                    for (int i = 0; i < cartCount; i++) {
                        f<<cart[i].product.name<<","<<cart[i].quantity<<"," 
                          <<(cart[i].product.price*cart[i].quantity)<<endl;
                    }
                    f<< "--------------------------"<<endl;
                    f.close();
                } 
				break;

                case 4:
                cout<<endl;
				return;
                default:
				cout<<"\nInvalid choice."<<endl;
		}
            
        } while (true);
    }
};

int main() {
	
	cout<<"\t\t\t\t\t********************************"<<endl;
    cout<<"\t\t\t\t\t*=== Well Come to SmartShop ===*"<<endl;
	cout<<"\t\t\t\t\t********************************"<<endl;
    int choice;
    	
    do
	{
		cout<<"-------------------"<<endl;
        cout<<"1. Customer Panel  "<<endl;
        cout<<"2. Admin Panel     "<<endl;
        cout<<"3. Exit            "<<endl;
        cout<<"-------------------"<<endl;
        cout<<"Enter your choice: ";
        cin>>choice;

        if(choice == 1)
		{
            Customer c;
            c.panel();
        } 
		else if(choice == 2)
		{
            Admin a;
            a.panel();
        } 
		else if(choice==3)
		{
            cout<<"Thank you for using SmartShop!"<<endl;
            break;
        } 
		else
		{
            cout<<"Invalid choice."<<endl;
        }
    } while(true);

    return 0;
}

