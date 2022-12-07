#include <iostream>
#include <algorithm>
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
template <class T>
void PrintVector(vector<T> &temp)
{
    for (int i = 0; i < temp.size(); i++)
    {
        cout << temp[i] << endl;
    }
}
int Max(int, int);
void UpdateWhoLikedMovie(MovieObject *, UserObject *);
void UpdateTheAverage(MovieObject *, UserObject *);
MovieObject *GetMovie(MovieObject *, int);
MovieObject *GetMovie(MovieObject *, float);
MovieObject *GetMovie(MovieObject *, string);
UserObject *GetUser(UserObject *, int);
void CollectedMovieData(vector<MovieObject *> &, MovieObject *);
void Averager(MovieObject *, float, vector<float> &);
void SaveFavourites(vector<string> &, MovieData);
void MakeUnique(vector<string> &);

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
    MovieData(int i)
    {
        this->root = NULL;
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
    void Push(MovieObject newMovie, float rating)
    {
        if (root == NULL)
        {
            root = new MovieObject(newMovie.MovieID, newMovie.name);
            root->avg_rating = rating;
        }
        else
        {
            MovieObject *temp = root;
            while (true)
            {
                if (newMovie.MovieID > temp->MovieID)
                {
                    if (temp->right != NULL)
                    {
                        temp = temp->right;
                    }
                    else
                    {
                        temp->right = new MovieObject(newMovie.MovieID, newMovie.name);
                        temp->right->avg_rating = rating;
                        break;
                    }
                }
                else if (newMovie.MovieID < temp->MovieID)
                {
                    if (temp->left != NULL)
                    {
                        temp = temp->left;
                    }
                    else
                    {
                        temp->left = new MovieObject(newMovie.MovieID, newMovie.name);
                        temp->left->avg_rating = rating;
                        break;
                    }
                }
                else
                {
                    temp->avg_rating += rating;
                    break;
                }
            }
        }
    }
    void FavSelection(MovieObject *root, vector<string> &temp)
    {
        if (root != NULL)
        {
            FavSelection(root->left, temp);
            FavSelection(root->right, temp);
            for (int i = 0; i < root->genre.size(); i++)
            {
                temp.push_back(root->genre[i]);
            }
        }
        MakeUnique(temp);
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
    void SelectFav(vector<string> temp, vector<string> &AllFav)
    {
        system("cls");
        int choice, i;
        while (AllFav.size() != 3)
        {
            system("cls");
            cout << "\t\t======================================================" << endl
                 << "\t\t\tMOVIE RECOMMENDATION SYSTEM (M.R.S.)" << endl
                 << "\t\t======================================================" << endl;
            for (i = 1; i < temp.size(); i += 2)
            {
                cout << "\t\t   " << i << ") " << temp[i] << "\t\t" << i + 1 << ") " << temp[i + 1] << endl;
            }
            cout << "\n\t\tEnter the option of your favourite genre (Select Any Three): ";
            fflush(stdin);
            cin >> choice;
            if (choice >= 1 && choice <= temp.size())
            {
                AllFav.push_back(temp[choice]);
                MakeUnique(AllFav);
            }
        }
    }

    void MovieSelect(vector<string> Fav, MovieObject *node, vector<int> &types1, vector<int> &types2, vector<int> &types3)
    {
        bool option1, option2, option3;

        if (node == NULL)
        {
            return;
        }

        // initializing the options
        option1 = false;
        option2 = false;
        option3 = false;

        // checking which are present in the movie genre
        for (int i = 0; i < node->genre.size(); i++)
        {
            if (Fav[0] == node->genre[i])
            {
                option1 = true;
            }
            else if (Fav[1] == node->genre[i])
            {
                option2 = true;
            }
            else if (Fav[2] == node->genre[i])
            {
                option3 = true;
            }
        }

        // pushing in the vectors based on the amount
        if (option1 && option2 && option3)
        {
            types3.push_back(node->MovieID);
        }
        else if ((option1 && option2) || (option3 && option2) || (option1 && option3))
        {
            types2.push_back(node->MovieID);
        }
        else if (option1 || option2 || option3)
        {
            types1.push_back(node->MovieID);
        }

        // calling the child nodes for the same function
        MovieSelect(Fav, node->left, types1, types2, types3);
        MovieSelect(Fav, node->right, types1, types2, types3);
    }

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

    // Function(s)
    void InsertionSort(vector<int> &temp)
    {
        for (int i = 1; i < temp.size(); i++)
        {
            for (int j = i; j < temp.size(); j++)
            {
                MovieObject *movie1 = GetMovie(MyMoviesData.root, temp[j]);
                MovieObject *movie2 = GetMovie(MyMoviesData.root, temp[j - 1]);
                if (movie1->avg_rating > movie2->avg_rating)
                {
                    int temporary = temp[j - 1];
                    temp[j - 1] = temp[j];
                    temp[j] = temporary;
                }
                else
                {
                    break;
                }
            }
        }
    }
    void CollaborativeSearch(string nameOfTheMovie, vector<MovieObject *> &MoviesList)
    {
        MovieObject *GivenMovie = GetMovie(MyMoviesData.root, nameOfTheMovie);
        if (GivenMovie == NULL)
        {
            cout << "\t\tSorry, But there is no such movie in our database. (! - !)" << endl;
            return;
        }
        vector<UserObject *> SimilarUsers;

        // Getting similar users if they like the given movie.
        for (int i = 0; i < GivenMovie->WhoRatedThisMovie.size(); i++)
        {
            UserObject *TempUserProfile = GetUser(MyUsersData.root, GivenMovie->WhoRatedThisMovie[i]);
            for (int j = 0; j < TempUserProfile->movieID.size(); j++)
            {
                if (TempUserProfile->movieID[j] == GivenMovie->MovieID && TempUserProfile->movieRating[j] >= 3.0)
                {
                    SimilarUsers.push_back(TempUserProfile);
                }
            }
        }

        if (SimilarUsers.size() == 0)
        {
            // cout << "Bagh Jao" << endl;
            return;
        }

        MovieData MovieSelectorTree(69);

        for (int i = 0; i < SimilarUsers.size(); i++)
        {
            for (int j = 0; j < SimilarUsers[i]->movieRating.size(); j++)
            {
                MovieObject *temp = GetMovie(MyMoviesData.root, SimilarUsers[i]->movieID[j]);
                if (temp == NULL)
                {
                    exit(4546465);
                }
                MovieObject temp2 = MovieObject(temp->MovieID, temp->name);
                MovieSelectorTree.Push(temp2, SimilarUsers[i]->movieRating[j]);
            }
        }
        vector<float> MoviesListRating;
        Averager(MovieSelectorTree.root, float(SimilarUsers.size()), MoviesListRating);

        // vector<MovieObject *> MoviesList;
        CollectedMovieData(MoviesList, MovieSelectorTree.root);

        // Insertion Sort
        for (int i = MoviesList.size() - 1; i >= 0 && i - 1 >= 0; i--)
        {
            for (int j = i; j < MoviesList.size(); j++)
            {
                if (MoviesList[j - 1]->avg_rating > MoviesList[j]->avg_rating)
                {
                    MovieObject *temp = MoviesList[j - 1];
                    MoviesList[j - 1] = MoviesList[j];
                    MoviesList[j] = temp;
                }
                else
                {
                    break;
                }
            }
        }
    }
    void Initialize()
    {
        vector<string> AllGenre;
        vector<string> Favourite;
        vector<int> types1;
        vector<int> types2;
        vector<int> types3;
        MyMoviesData.FavSelection(MyMoviesData.root, AllGenre);
        MyUsersData.SelectFav(AllGenre, Favourite);
        cout << endl
             << endl;
        MyUsersData.MovieSelect(Favourite, MyMoviesData.root, types1, types2, types3);
        InsertionSort(types1);
        InsertionSort(types2);
        InsertionSort(types3);

        vector<int> SelectedMovies;
        for (int i = 0; i < types3.size(); i++)
        {
            SelectedMovies.push_back(types3[i]);
        }
        for (int i = 0; i < types2.size(); i++)
        {
            SelectedMovies.push_back(types2[i]);
        }
        for (int i = 0; i < types1.size(); i++)
        {
            SelectedMovies.push_back(types1[i]);
        }

        vector<MovieObject *> MoviesList;
        string nameOfTheMovie;
        cout << "\t\tEnter name of your favorite movie: ";
        fflush(stdin);
        getline(cin, nameOfTheMovie);
        CollaborativeSearch(nameOfTheMovie, MoviesList);

        cout << "\t";
        for (int i = 0; i < 20; i++)
        {
            cout << "====";
        }
        cout << endl
             << endl;
        cout << "\t";
        for (int i = 0; i < 8; i++)
        {
            cout << "----";
        }
        // Printing Content Based Data
        cout << endl
             << "\tRECOMMENDED FOR YOU `\\( ^ w ^ )/`" << endl;
        cout << "\t";
        for (int i = 0; i < 8; i++)
        {
            cout << "----";
        }
        cout << endl
             << "\tGENRE:\t";
        for (int i = 0; i < Favourite.size(); i++)
        {
            cout << Favourite[i] << "   ";
        }
        cout << endl
             << endl;
        for (int i = 0; i < SelectedMovies.size() && i < 11; i++)
        {
            MovieObject *temp = GetMovie(MyMoviesData.root, SelectedMovies[i]);
            cout << "\t" << temp->name << endl;
        }
        cout << endl;
        if (MoviesList.size() != 0)
        {
            cout << "\t";
            for (int i = 0; i < 20; i++)
            {
                cout << "====";
            }
            cout << endl
                 << endl;
            cout << "\t";
            for (int i = 0; i < 8; i++)
            {
                cout << "----";
            }

            cout << endl
                 << "\tPEOPLE ALSO LIKED `\\( $ O $ )/`" << endl;
            cout << "\t";
            for (int i = 0; i < 8; i++)
            {
                cout << "----";
            }
            cout << endl
                 << endl;
            // Printing Collaborative Data
            int y = MoviesList.size() - 1 - 10;
            int z = MoviesList.size() - 1 - y;
            for (int i = z; i >= 0; i--)
            {
                cout << "\t" << MoviesList[i]->name << endl;
            }
        }
        else
        {
            cout << "\t\tCongratulation Buddy !!! You Are Unique `\\(> O <)/`" << endl;
        }
        cout << endl
             << "\t";
        for (int i = 0; i < 20; i++)
        {
            cout << "====";
        }
        cout << endl
             << endl;
    }
};

int main(int argc, char const *argv[])
{
    Dataset MyDataset;
    MyDataset.Initialize();
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
            MovieObject *tempMovie = GetMovie(M, U->movieID[i]);
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
            M->avg_rating = 0.0;
        }
        else
        {
            float tempTotal = 0;
            for (int i = 0; i < M->WhoRatedThisMovie.size(); i++)
            {
                UserObject *TempUserProfile = GetUser(U, M->WhoRatedThisMovie[i]);
                for (int j = 0; j < TempUserProfile->movieID.size(); j++)
                {
                    if (TempUserProfile->movieID[j] == M->MovieID)
                    {
                        tempTotal += TempUserProfile->movieRating[j];
                        break;
                    }
                }
            }
            M->avg_rating = tempTotal / float(M->WhoRatedThisMovie.size());
        }
        UpdateTheAverage(M->left, U);
        UpdateTheAverage(M->right, U);
    }
}

MovieObject *GetMovie(MovieObject *MoviesData, int ID)
{

    MovieObject *temp = MoviesData;
    while (true && temp != NULL)
    {
        if (temp->MovieID == ID)
        {
            return temp;
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
    return NULL;
}
MovieObject *GetMovie(MovieObject *MoviesData, float rating)
{
    MovieObject *temp = MoviesData;
    while (true && temp != NULL)
    {
        if (temp->avg_rating == rating)
        {
            return temp;
        }
        else if (temp->avg_rating > rating)
        {
            temp = temp->left;
        }
        else
        {
            temp = temp->right;
        }
    }
    return NULL;
}
MovieObject *GetMovie(MovieObject *MoviesData, string nameOfTheMovie)
{
    vector<MovieObject *> temp;
    CollectedMovieData(temp, MoviesData);
    for (int i = 0; i < temp.size(); i++)
    {
        if (temp[i]->name == nameOfTheMovie)
        {
            return GetMovie(MoviesData, temp[i]->MovieID);
        }
    }
    return NULL;
}

UserObject *GetUser(UserObject *UserData, int ID)
{

    UserObject *temp = UserData;
    while (true && temp != NULL)
    {
        if (temp->UserID == ID)
        {
            return temp;
        }
        else if (temp->UserID > ID)
        {
            temp = temp->left;
        }
        else
        {
            temp = temp->right;
        }
    }
    return NULL;
}

void CollectedMovieData(vector<MovieObject *> &O, MovieObject *M)
{
    if (M != NULL)
    {

        O.push_back(M);
        CollectedMovieData(O, M->left);
        CollectedMovieData(O, M->right);
    }
}
void Averager(MovieObject *root, float divisor, vector<float> &MoviesList)
{
    if (root != NULL)
    {
        root->avg_rating /= divisor;
        MoviesList.push_back(root->avg_rating);
        Averager(root->left, divisor, MoviesList);
        Averager(root->right, divisor, MoviesList);
    }
    return;
}

void MakeUnique(vector<string> &temp)
{
    sort(temp.begin(), temp.end());
    vector<string>::iterator ip;
    ip = unique(temp.begin(), temp.end());
    temp.erase(ip, temp.end());
}
