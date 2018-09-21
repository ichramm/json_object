ROOT=.
CXX=g++

function search {
local node=$1
local level=$2

# Determine if this node has been covered
if [[ ! "$nodes" =~ "$node" ]]; then
# Keep track of the nodes that have been already searched
    nodes="$nodes $node"
        
    #echo "File: $node"
    for header in `$CXX -I$ROOT/IFC -I$ROOT -I$ROOT/src $CXXFLAGS $node -MM | sed -e 's/ /\n/g' | grep '.h$' | grep 'src' | sort | uniq`; do 
        if [ -f ${header%.h}.cpp ]; then 
            object=${header%.h}.o;

			# Print out the current dependency            
            echo -e "$level $object"
            
            # Enter the next level in the tree
			search ${object%.o}.cpp $level"**"             
        fi; 
    done; 
fi
}

# Entry point for printing the dependency tree
root="$1"

# Recursivly print the entire dependency tree
search $root "*"
