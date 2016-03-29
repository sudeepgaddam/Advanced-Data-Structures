#include "red_black.h"

/*
 * getFilesize
 * @inputs: const char* filename
 * @return: void
 * @Description: Given a file name, Returns the size of file, used in mmaping the input file
 *
 */
size_t getFilesize(const char* filename) {
    struct stat st;
    stat(filename, &st);
    return st.st_size;
}


/*
 * readCommands
 * @inputs: RBTree<int> &tree
 * @return: void
 * @Description: Used to read commands from command line or input file,
 * Reads till it finds quit command
 * Calls corresponding functions depending on the commands
 *
 */
void readCommands(RBTree<int> &tree) {
	string s;
	//cin>>s;
	int first=0, second=0;
	while(1) {
			cin>>s;

		if(s=="increase"){
			cin>>first>>second;
			tree.Increase(first, second);
		} else if(s=="reduce") {
			//cout<<"Reducing" << endl;
			cin>>first>>second;
			tree.Reduce(first, second);
			
		}
		else if(s=="count") {
			cin>>first;
			tree.Count(first);
		} 
		else if(s == "inrange") {
			cin>>first>>second;
			tree.InRange(first, second);

			
		} else if(s=="next") {
			cin>>first;
			tree.Next(first);
			
		} else if(s=="previous")  {
					//	cout<<"Previous" << endl;

			cin>>first;
			tree.Previous(first);
			
		} else{ 
			return;
			}
		
	}
	
	
}/*
 * readSortedArraytoBst
 * @inputs: const char* argv,RBTree<int> &t,RBNode<int> *dummy
 * @return: void
 * @Description: Reads data from file and constructs vector and then calls initialise of RBTree
 * It will construct BST from array
 *
 */
void readSortedArraytoBst(const char* argv,RBTree<int> &t,RBNode<int> *dummy){
	std::ifstream infile;
	infile.open(argv);
	int a, b;
	infile>> a;
	//RBNode<T> *node;
	vector<RBNode<int> *> abc(a);
	int i=0;
	
	while (infile >> a >> b)
	{
		abc[i] = new RBNode<int>(a,b,dummy,dummy);
		i++;
	}
	//cout << abc.size() << endl;
	t.initialise(abc);
	//cout << t.root->id << endl;
	
}/*
 * readInputIfStream
 * @inputs: const char* argv,RBTree<int> &t,RBNode<int> *dummy
 * @return: void
 * @Description: Reads data from text file and then inserts node one by one
 *
 */
void readInputIfStream(const char* argv,RBTree<int> &t,RBNode<int> *dummy){
	std::ifstream infile;
	infile.open(argv);
	int a, b;
	infile>> a;
	RBNode<int> *node;

	while (infile >> a >> b)
	{
			node = new RBNode<int>(a,b,dummy,dummy);
			t.RBInsert(node);
	}
}
/*
 * readInputMMap
 * @inputs: string argv,RBTree<int> &t,RBNode<int> *dummy
 * @return: void
 * @Description: Reads input from text file using mmap system call, 
 * It is an alternative way of reading input from file
 *
 */
void readInputMMap(string argv,RBTree<int> &t,RBNode<int> *dummy) {
	RBNode<int> *node;
	size_t filesize = getFilesize(argv.c_str());
	int fd = open(argv.c_str(), O_RDONLY, 0);
    assert(fd != -1);
    //Execute mmap
    char* mmappedData =(char *) mmap(NULL, filesize, PROT_READ, MAP_PRIVATE | MAP_POPULATE, fd, 0);
    assert(mmappedData != MAP_FAILED);
    char c;
    stringstream ss;
	size_t i = 0; 
		
	while (mmappedData[i]!='\n' ) {
		ss<<mmappedData[i];
		i++;
	}
	int nums = stoi(ss.str());
	ss.str("");
	int first, second;
    while ( i++ <= filesize) {
        c = mmappedData[i];
        if (c == '\n') {
			second = stoi(ss.str());
			 node = new RBNode<int>(first,second,dummy,dummy);
			t.RBInsert(node);

			ss.str("");
			
        } else if(c == ' ') {
			first = stoi(ss.str());
			ss.str("");
		}
        else {
            // c style tokenizing
            ss << c;
        }

    }
}
int main(int argc, char* argv[])
{

    RBNode<int> *node;
    RBNode<int>  *dummy = new RBNode<int>(DUMMY_VALUE,DUMMY_VALUE,NULL,NULL);
    dummy->red = false;

    RBTree<int> t(dummy);
    
	if(argc<2){
		cout<<"Give Input file" << endl;
		exit(1);
	}
	string str(argv[1]);
	//readInputIfStream(argv[1],t,dummy);
    readSortedArraytoBst(argv[1],t,dummy);
    t.countBlackNodes();
	//If RB properties valid, then read commands, Else bail out
	if(	t.testRBProperties(t.root, 0)) readCommands(t);
	else cout<<"RB Tree properties not valid" << endl;
	
	delete dummy;
	
	//	    t.printTree();

    
    
}
