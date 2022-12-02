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
    int ID;
    vector<int> movieID;
    vector<int> movieRating;
    UserObject *left, *right;

};

int main(int argc, char const *argv[])
{
    MovieData MyMoviesData;
    MyMoviesData.PrintData(MyMoviesData.root);
    return 0;
}