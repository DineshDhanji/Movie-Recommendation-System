#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;

// GLOBAL FUNCTIONS
int Max(int a, int b)
{
    return a > b ? a : b;
}

class MovieObject
{
public:
    int ID;
    string name;
    vector<string> genre;
    vector<int> WhoRatedThisMovie;
    MovieObject *left, *right;

    // Constructor(s)
    MovieObject(int ID, string name)
    {
        this->ID = ID;
        this->name = name;
        this->left = this->right = NULL;
    }
    ~MovieObject()
    {
        delete left, right;
    }
};
class MovieData
{
public:
    MovieObject *root;

    // Constructor(s)
    MovieData()
    {
        // string addressOfFile = "TempMovies.csv";
        string line, data;
        ifstream file("Book1.csv");
        if (file.is_open())
        {
            while (getline(file, line))
            {
                stringstream s(line);

                int TempID;
                string TempName;

                // // Getting ID
                getline(s, data, ',');
                stringstream ss;
                ss << data;
                ss >> TempID;

                // // Getting Name
                getline(s, data, ',');
                TempName = data;

                MovieObject *temp = new MovieObject(TempID, TempName);

                while (getline(s, data, ','))
                {
                    temp->genre.push_back(data);
                }

                this->root = Insert(root, temp);
                // break;
            }
        }
        else
        {
            // root = NULL;
            cout << "File Not Found" << endl;
            exit(404);
        }
        file.close();
    }
    ~MovieData()
    {
        delete root;
    }

    // Function(s)
    int GetHeight(MovieObject *temp)
    {
        if (temp != NULL)
        {
            return (GetHeight(temp->left), GetHeight(temp->right)) + 1;
        }
        return -1;
    }

    MovieObject *LeftRotate(MovieObject *X)
    {
        MovieObject *Y = X->right;
        MovieObject *T2 = Y->left;
        Y->left = X;
        X->right = T2;
        return Y;
    }
    MovieObject *RightRotate(MovieObject *Y)
    {
        MovieObject *X = Y->left;
        MovieObject *T2 = X->right;
        X->right = Y;
        Y->left = T2;
        return X;
    }

    MovieObject *Insert(MovieObject *temp, MovieObject *newMovie)
    {
        // Inserting new Node
        if (temp == NULL)
        {
            return newMovie;
        }
        else
        {
            if (newMovie->ID < temp->ID)
            {
                if (temp->left == NULL)
                {
                    temp->left = newMovie;
                }
                else
                {
                    temp->left = Insert(temp->left, newMovie);
                }
            }
            else
            {
                if (temp->right == NULL)
                {
                    temp->right = newMovie;
                }
                else
                {
                    temp->right = Insert(temp->right, newMovie);
                }
            }
        }

        int balanceFactor = GetHeight(temp->left) - GetHeight(temp->right);
        // Left Case
        if (balanceFactor > 1 && newMovie->ID < temp->left->ID)
        {
            temp = RightRotate(temp);
        }
        // Right Case
        if (balanceFactor < -1 && newMovie->ID > temp->right->ID)
        {
            temp = LeftRotate(temp);
        }
        // Left -> Right Case
        if (balanceFactor > 1 && newMovie->ID > temp->left->ID)
        {
            temp->left = LeftRotate(temp->left);
            return RightRotate(temp);
        }
        // Right -> Left Case
        if (balanceFactor < -1 && newMovie->ID < temp->right->ID)
        {
            temp->right = RightRotate(temp->right);
            return LeftRotate(temp->left);
        }
        return temp;
    }

    void PrintData(MovieObject *root)
    {
        if (root != NULL)
        {
            cout << root->ID << " " << root->name << endl;
            PrintData(root->left);
            PrintData(root->right);
        }
    }
};
class UserObject
{
public:
    int UserID;
    vector<int> movieID;
    vector<int> movieRating;
    UserObject *left, *right;

    // Constructor(s)
    UserObject(int ID)
    {
        this->UserID = ID;
        this->left = this->right = NULL;
    }
    ~UserObject()
    {
        delete left, right;
    }
};
class UserData
{
public:
    UserObject *root;

    // Constructor(s)
    UserData()
    {
        string line, data;
        ifstream file("Book2.csv");
        int previoususerID;
        if (file.is_open())
        {
            while (getline(file, line))
            {
                stringstream s(line);
                int TempUserID, TempMovieID;
                float TempUserRating;
                getline(s, data, ',');
                stringstream s1;
                s1 << data;
                s1 >> TempUserID;

                cout<<"Temp User ID "<<TempUserID<<endl;
                if (previoususerID == TempUserID)
                {

                }
                
                UserObject *temp = new UserObject(TempUserID);

                getline(s, data, ',');
                stringstream s2;
                s2 << data;
                s2 >> TempMovieID;
                cout<<"Temp movie ID "<<TempMovieID<<endl;

                getline(s, data, ',');
                stringstream s3;
                s3 << data;
                s3 >> TempUserRating;
                cout<<"Temp User Rating "<<TempUserRating<<endl;

                temp->movieID.push_back(TempMovieID);
                temp->movieRating.push_back(TempUserRating);
                previoususerID = TempUserID;
                this->root = Insert(root, temp);
                // break;
            }
        }
        else
        {
            // root = NULL;
            cout << "File Not Found" << endl;
            exit(404);
        }
        file.close();
    }
    ~UserData()
    {
        delete root;
    }

    // Function(s)
    int GetHeight(UserObject *temp)
    {
        if (temp != NULL)
        {
            return (GetHeight(temp->left), GetHeight(temp->right)) + 1;
        }
        return -1;
    }

    UserObject *LeftRotate(UserObject *X)
    {
        UserObject *Y = X->right;
        UserObject *T2 = Y->left;
        Y->left = X;
        X->right = T2;
        return Y;
    }
    UserObject *RightRotate(UserObject *Y)
    {
        UserObject *X = Y->left;
        UserObject *T2 = X->right;
        X->right = Y;
        Y->left = T2;
        return X;
    }

    UserObject *Insert(UserObject *temp, UserObject *newUser)
    {
        // Inserting new Node
        if (temp == NULL)
        {
            return newUser;
        }
        else
        {
            // if (newUser->UserID == temp->UserID)
            // {
            //     temp->movieID =
            // }
            
            if (newUser->UserID < temp->UserID)
            {
                if (temp->left == NULL)
                {
                    temp->left = newUser;
                }
                else
                {
                    temp->left = Insert(temp->left, newUser);
                }
            }
            else
            {
                if (temp->right == NULL)
                {
                    temp->right = newUser;
                }
                else
                {
                    temp->right = Insert(temp->right, newUser);
                }
            }
        }

        int balanceFactor = GetHeight(temp->left) - GetHeight(temp->right);
        // Left Case
        if (balanceFactor > 1 && newUser->UserID < temp->left->UserID)
        {
            temp = RightRotate(temp);
        }
        // Right Case
        if (balanceFactor < -1 && newUser->UserID > temp->right->UserID)
        {
            temp = LeftRotate(temp);
        }
        // Left -> Right Case
        if (balanceFactor > 1 && newUser->UserID > temp->left->UserID)
        {
            temp->left = LeftRotate(temp->left);
            return RightRotate(temp);
        }
        // Right -> Left Case
        if (balanceFactor < -1 && newUser->UserID < temp->right->UserID)
        {
            temp->right = RightRotate(temp->right);
            return LeftRotate(temp->left);
        }
        return temp;
    }

    void PrintData(UserObject *root)
    {
        if (root != NULL)
        {
            cout << root->UserID << " " << endl;
            PrintData(root->left);
            PrintData(root->right);
        }
    }
};

// class UserObject
// {
// public:
//     int ID;
//     vector<int> movieID;
//     vector<int> movieRating;
//     UserObject *left, *right;

// };

int main(int argc, char const *argv[])
{
    MovieData MyMoviesData;
    MyMoviesData.PrintData(MyMoviesData.root);
    cout<<endl<<endl;
    UserData MyUsersData;
    MyUsersData.PrintData(MyUsersData.root);
    return 0;
}
