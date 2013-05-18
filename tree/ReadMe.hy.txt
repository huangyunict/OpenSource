This project is available at: http://tree.phi-sci.com/

Version is tree-2.81.tar.gz

Since this is a light-weight header-file-only project, I move the contents in original ``src'' directory into current directory.

I wrap the original test case ``test_tree.cc'' in ``run_test.sh''.

I add a special macro in the header file to indicate whether the header has been included.

    tree.hh:31:  #define NSVIRGO_NSTREE      //  define header macro

