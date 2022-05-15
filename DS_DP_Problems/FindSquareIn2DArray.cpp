/* Find size of largest square sub-matrix of 1’s present in given binary matrix
*/

#include <iostream>
using namespace std;

// M x N matrix
#define M 8
#define N 6

/* Solution 1: space optimal, Time : O(M*N) space: O(1)
 * recursion call top to bottom, base condition at edge 0
 * calculate square size at each position
 */
int findLargestSquare(int mat[M][N], int r, int c, int& max_size){
		/* base condition */
		if(r==0 || c==0){
			cout<<"DEUBG, base condition hit, max_size= "<<max_size;
			cout<<", r,c="<<r<<","<<c<<" , mat[r][c]: "<<mat[r][c]<<endl;
			if(max_size != 0){
				max_size = max(max_size, mat[r][c]);
				return mat[r][c];
			}

			for(int i=0; i<=r; i++){
				if(mat[i][c] == 1){
					max_size=1;
					break;
				}
			}

			for(int j=0; j<=c; j++){
				if(mat[r][j] == 1){
					max_size=1;
					break;
				}
			}

			return max_size;
		}

		/* find for top, left, and top left */
		/* left r, c-1 */
		int left=findLargestSquare(mat, r, c-1, max_size);

		/* top r-1, c */
		int top=findLargestSquare(mat, r-1, c, max_size);

		/* top-left r-1, c-1 */
		int top_left=findLargestSquare(mat, r-1, c-1, max_size);

		cout<<"DEUBG, r,c: "<<r<<","<<c<<", mat[r][c]: "<<mat[r][c]<<endl;

		/* find max among found values
		 * if there is 1 at current position */
		int size=0;
		if(mat[r][c]==1){
			size=1+min( min(top, left), top_left);
		}

        /* update max size found so far */
		max_size=max(max_size, size);
		cout<<"size: "<<size<<", max_size: "<<max_size<<endl;

		return size;
}

/* Solution 2: time optimal, Dynamic Programming
 * Time O(M+N), space O(M*N)
 * memorize suqare from bottom to top, no recursion call
 */
int findLargestSquareDP(int mat[M][N]){
     /* lookup matrix stores suqare found till
      * and that position
      */
	int lookup[M][N];

	int max_size=0;  // store max size

	/* fill lookup bottom-up, and calculate size at each step */
	for(int r=0; r<M; r++){
		for(int c=0; c<N; c++){
             lookup[r][c]=mat[r][c];

             /* if no 0th row & column, and
              * position has 1 in it, then calculate possible matrix
              * top, left, top-left
              */
             if(r && c && mat[r][c]==1)
            	 lookup[r][c]=1 + min( min(lookup[r-1][c], lookup[r][c-1]),
            			                lookup[r-1][c-1]);

             cout<<"DEBUG: r,c: "<<r<<","<<c<<", lookup[r][c]: "<<lookup[r][c]<<endl;
             /* update max if greater value found */
             max_size=max(max_size, lookup[r][c]);
		}
	}

	return max_size;
}

int main() {
	cout << "---CPP DP Matrix---" << endl;
	cout<<"Finding largest square size in matrix"<<endl;

	/*int mat[M][N] =
	{
	   { 1, 1, 1, 0},
	   { 1, 1, 1, 0},
	   { 1, 1, 1, 0},
	   { 0, 0, 0, 0}
	};*/
    int mat[M][N] =
    {
        { 0, 0, 1, 0, 1, 1 },
        { 0, 1, 1, 1, 0, 0 },
        { 0, 0, 1, 1, 1, 1 },
        { 1, 1, 0, 1, 1, 1 },
        { 1, 1, 1, 1, 1, 1 },
        { 1, 1, 1, 1, 1, 1 },
        { 1, 0, 1, 1, 1, 1 },
        { 1, 1, 1, 1, 1, 1 }
    };


    // size stores the size of largest square sub-matrix of 1's
    // and it is passed by reference
    int max_size = 0;

    /*findLargestSquare(mat, M-1, N-1, max_size);*/

    max_size=findLargestSquareDP(mat);

    cout << "The size of largest square sub-matrix of 1's is: " << max_size <<endl;

    return 0;

}
