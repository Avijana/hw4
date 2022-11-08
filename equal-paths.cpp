#include "equal-paths.h"
using namespace std;



// You may add any prototypes of helper functions here

bool ePathsHelper(Node* root, int *leafLevel, int level)
{
  if(root == NULL)
  {
      return true;
  }
	if(root->left == NULL && root->right ==NULL)
	{
		if (*leafLevel == 0)
		{
			*leafLevel = level;
			return true;
		}
		else
		{
			return(*leafLevel==level);
		}
	}
	return ePathsHelper(root->left, leafLevel, level+1) && ePathsHelper(root->right, leafLevel, level+1);
}

bool equalPaths(Node * root)
{
    // Add your code below
    int leafLevel = 0;
		int level = 0;
		return ePathsHelper(root, &leafLevel, level);
}

