if [[ $# -eq 0 ]] ; then
    echo 'error: please enter a commit comment'
    exit 1
fi

find . -type f -name "*.b#*" -exec rm -f {} \;
# find . -type f -name "*.dsb" -exec rm -f {} \;
find . -type f -name "*.s#*" -exec rm -f {} \;
find . -type f -name "*.l#*" -exec rm -f {} \;
find . -type f -name "*.csv#*" -exec rm -f {} \;
find . -type f -name "*.DS_Store*" -exec rm -f {} \;
find . -type f -name "*.eggs*" -exec rm -f {} \;
find . -name "__pycache__" -exec rm -rf {} \;
find . -type f -name ".component_hash" -exec rm {} +

# cd ./resources/sample_profiles
# if [[ "$OSTYPE" == "msys" ]] ; then
#     python zip_sample.py
# else
#     python3 zip_sample.py
# fi

current_branch=$(git rev-parse --abbrev-ref HEAD)

git add --all
git commit -m "$@"
git push origin "$current_branch"
