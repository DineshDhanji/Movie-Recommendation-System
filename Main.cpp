#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;

// GLOBAL FUNCTION'S PROTOTYPE(S)
int Max(int, int);

class MovieObject
{
public:
    int MovieID;
    string name;
    float avg_rating;
    vector<string> genre;
    vector<int> WhoRatedThisMovie;
    MovieObject *left, *right;

    // Constructor(s)
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

    // Constructor(s)
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
            cout << root->MovieID << " " << root->name << " ";
            for (int i = 0; i < root->genre.size(); i++)
            {
                cout << root->genre[i] << ", ";
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

// void UpdatingWholikedmovie(MovieObject *root1, UserObject *root2)
// {
//     while (root2 != NULL)
//     {
//         int size = root2->movieID.size();
//         for (int i = 0; i < size; i++)
//         {
//             // MovieObject* temp = MyMoviesData.root;
//             while (root1 != NULL)
//             {
//                 if (root2->movieID[i] == root1->MovieID)
//                 {
//                     root1->WhoRatedThisMovie.push_back(root2->UserID);
//                     break;
//                 }
//                 else if (root2->movieID[i] < root1->MovieID)
//                 {
//                     root1 = root1->left;
//                 }
//                 else if (root2->movieID[i] > root1->MovieID)
//                 {
//                     root1 = root1->left;
//                 }
//             }
//         }
//         UpdatingWholikedmovie(root1, root2->left);
//         UpdatingWholikedmovie(root1, root2->right);
//     }
// }

// void Updating_average_rating_of_movie(MovieObject *root1, UserObject *root2)
// {
//     while (root1 != NULL)
//     {
//         int size = root1->WhoRatedThisMovie.size();
//         for (int i = 0; i < size; i++)
//         {
//             while (root2 != NULL)
//             {
//                 if (root1->WhoRatedThisMovie[i] == root2->UserID)
//                 {
//                     int size2 = root2->movieID.size();
//                     for (int i = 0; i < size2; i++)
//                     {
//                         if (root2->movieID[i] == root1->MovieID)
//                         {
//                             root1->avg_rating += root2->movieRating[i];
//                             break;
//                         }
//                     }
//                     break;
//                 }
//                 else if (root1->WhoRatedThisMovie[i] < root2->UserID)
//                 {
//                     root2 = root2->left;
//                 }
//                 else if (root1->WhoRatedThisMovie[i] > root2->UserID)
//                 {
//                     root2 = root2->right;
//                 }
//             }
//         }
//         Updating_average_rating_of_movie(root1->left, root2);
//         Updating_average_rating_of_movie(root1->right, root2);
//     }
// }

// void UpdatingWholikedmovie(MovieData MyMoviesData, UserData MyUsersData)
// {
//     // int user = MyUsersData.root->UserID;
//     newfunc(MyMoviesData.root, MyUsersData.root);
//     // while(MyUsersData.root != NULL)
//     // {
//     //     int size = MyUsersData.root->movieID.size();
//     //     for (int i = 0; i < size; i++)
//     //     {
//     //         // MovieObject* temp = MyMoviesData.root;
//     //         while (MyMoviesData.root != NULL)
//     //         {
//     //             if (MyUsersData.root->movieID[i] == MyMoviesData.root->MovieID)
//     //             {
//     //                 MyMoviesData.root->WhoRatedThisMovie.push_back(MyUsersData.root->UserID);
//     //                 break;
//     //             }
//     //             else if (MyUsersData.root->movieID[i] < MyMoviesData.root->MovieID)
//     //             {
//     //                 MyMoviesData.root = MyMoviesData.root->left;
//     //             }
//     //             else if (MyUsersData.root->movieID[i] > MyMoviesData.root->MovieID)
//     //             {
//     //                 MyMoviesData.root = MyMoviesData.root->left;
//     //             }
//     //         }
//     //     }
//     //     MyUsersData.root = MyUsersData.root->left;
//     }
//     }
// }
int main(int argc, char const *argv[])
{
    MovieData MyMoviesData;
    MyMoviesData.PrintData(MyMoviesData.root);
    cout << endl
         << endl;
    UserData MyUsersData;
    MyUsersData.PrintData(MyUsersData.root);
    // UpdatingWholikedmovie(MyMoviesData.root, MyUsersData.root);
    // Updating_average_rating_of_movie(MyMoviesData.root, MyUsersData.root);
    return 0;
}

// FUNCTION(S)
int Max(int a, int b)
{
    return a > b ? a : b;
}
