#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;

// CLASSES PROTOTYPE(S)
class MovieObject;
class MovieData;
class UserObject;
class UserData;
class Dataset;

// GLOBAL FUNCTION'S PROTOTYPE(S)
int Max(int, int);
void UpdateWhoLikedMovie(MovieObject *, UserObject *);
void UpdateTheAverage(MovieObject *, UserObject *);
MovieObject *GetMovieByID(MovieObject *, int);

class MovieObject
{
public:
    int MovieID;
    string name;
    float avg_rating;
    vector<string> genre;
    vector<int> WhoRatedThisMovie;
    MovieObject *left, *right;

    // Constructor(s) & Destructor
    MovieObject(int ID, string name)
    {
        this->MovieID = ID;
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

    // Constructor(s) & Destructor
    MovieData()
    {
        this->root = NULL;
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

                // Getting ID
                getline(s, data, ',');
                stringstream ss;
                ss << data;
                ss >> TempID;

                // Getting Name
                getline(s, data, ',');
                TempName = data;

                MovieObject *temp = new MovieObject(TempID, TempName);

                while (getline(s, data, ','))
                {
                    temp->genre.push_back(data);
                }

                this->root = Insert(root, temp);
            }
        }
        else
        {
            cout << "Movie File Not Found !!!" << endl;
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
            if (newMovie->MovieID < temp->MovieID)
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
        if (balanceFactor > 1 && newMovie->MovieID < temp->left->MovieID)
        {
            temp = RightRotate(temp);
        }
        // Right Case
        if (balanceFactor < -1 && newMovie->MovieID > temp->right->MovieID)
        {
            temp = LeftRotate(temp);
        }
        // Left -> Right Case
        if (balanceFactor > 1 && newMovie->MovieID > temp->left->MovieID)
        {
            temp->left = LeftRotate(temp->left);
            return RightRotate(temp);
        }
        // Right -> Left Case
        if (balanceFactor < -1 && newMovie->MovieID < temp->right->MovieID)
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
            cout << root->MovieID << " " << root->name << " R" << root->avg_rating << " ";
            for (int i = 0; i < root->genre.size(); i++)
            {
                cout << root->genre[i] << ", ";
            }
            for (int i = 0; i < root->WhoRatedThisMovie.size(); i++)
            {
                cout << root->WhoRatedThisMovie[i] << "  ";
            }

            cout << endl;
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
    vector<float> movieRating;
    UserObject *left, *right;

    // Constructor(s) & Destructor
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

    // Constructor(s) & Destructor
    UserData()
    {
        this->root = NULL;
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

                // Getting ID
                getline(s, data, ',');
                stringstream s1;
                s1 << data;
                s1 >> TempUserID;

                // Getting Movie's ID
                getline(s, data, ',');
                stringstream s2;
                s2 << data;
                s2 >> TempMovieID;

                // Getting Movie's Rating
                getline(s, data, ',');
                stringstream s3;
                s3 << data;
                s3 >> TempUserRating;

                UserObject *temp = new UserObject(TempUserID);

                temp->movieID.push_back(TempMovieID);
                temp->movieRating.push_back(TempUserRating);
                this->root = Insert(root, temp);
            }
        }
        else
        {
            // root = NULL;
            cout << "User File Not Found !!!" << endl;
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
        if (temp == NULL || newUser == NULL)
        {
            return newUser;
        }
        else
        {
            if (newUser->UserID == temp->UserID)
            {
                temp->movieID.push_back(newUser->movieID.front());
                temp->movieRating.push_back(newUser->movieRating.front());
            }
            else if (newUser->UserID < temp->UserID)
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
            PrintData(root->left);
            cout << root->UserID << " ";
            for (int i = 0; i < root->movieID.size(); i++)
            {
                cout << root->movieID[i] << "|"
                     << root->movieRating[i] << "  ";
            }
            cout << endl;
            PrintData(root->right);
        }
    }
};

class Dataset
{
public:
    MovieData MyMoviesData;
    UserData MyUsersData;

    // Constructor(s) & Destructor
    Dataset()
    {
        UpdateWhoLikedMovie(MyMoviesData.root, MyUsersData.root);
        UpdateTheAverage(MyMoviesData.root, MyUsersData.root);
    }
    ~Dataset()
    {
    }
};

int main(int argc, char const *argv[])
{
    // MovieData MyMoviesData;
    // MyMoviesData.PrintData(MyMoviesData.root);
    // cout << endl
    //      << endl;
    // UserData MyUsersData;
    // MyUsersData.PrintData(MyUsersData.root);

    Dataset MyDataset;
    MyDataset.MyMoviesData.PrintData(MyDataset.MyMoviesData.root);
    cout << endl
         << endl;
    MyDataset.MyUsersData.PrintData(MyDataset.MyUsersData.root);

    return 0;
}

// FUNCTION(S)
int Max(int a, int b)
{
    return a > b ? a : b;
}

void UpdateWhoLikedMovie(MovieObject *M, UserObject *U)
{
    if (M != NULL && U != NULL)
    {
        for (int i = 0; i < U->movieID.size(); i++)
        {
            MovieObject *tempMovie = GetMovieByID(M, U->movieID[i]);
            if (tempMovie == NULL)
            {
                continue;
            }
            tempMovie->WhoRatedThisMovie.push_back(U->UserID);
        }
        UpdateWhoLikedMovie(M, U->left);
        UpdateWhoLikedMovie(M, U->right);
    }
}
void UpdateTheAverage(MovieObject *M, UserObject *U)
{
    if (M != NULL && U != NULL)
    {
        if (M->WhoRatedThisMovie.size() == 0)
        {
            M->avg_rating = 0;
        }
        else
        {
            float tempTotal = 0;
            for (int i = 0; i < M->WhoRatedThisMovie.size(); i++)
            {
                tempTotal += M->WhoRatedThisMovie[i];
            }
            M->avg_rating = tempTotal / float(M->WhoRatedThisMovie.size());
        }
        UpdateTheAverage(M->left, U);
        UpdateTheAverage(M->right, U);
    }
}

MovieObject *GetMovieByID(MovieObject *MovieData, int ID)
{

    MovieObject *temp = MovieData;
    while (true && temp != NULL)
    {
        if (temp->MovieID == ID)
        {
            break;
        }
        else if (temp->MovieID > ID)
        {
            temp = temp->left;
        }
        else
        {
            temp = temp->right;
        }
    }
    return temp;
}
