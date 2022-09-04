# .bash_profile

# Get the aliases and functions
if [ -f ~/.bashrc ]; then
	. ~/.bashrc
fi

# User specific environment and startup programs

export RUN_PATH=$HOME

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$RUN_PATH/bin:$RUN_PATH/lib:/usr/local/lib64/

export PATH=$PATH:$RUN_PATH/bin:$RUN_PATH/src/bin:.

ulimit  -c unlimited

PS1="\[\e[33m\]\u \[\e[36m\]\w \[\e[34m\]\\$ \[\e[0m\]"

# 使用 g++ 9
#source scl_source enable devtoolset-9
