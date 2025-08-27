for f in *.hpp
    set -l name (string split -m1 -r '.' "$f")[1]
    mv {$name}.hpp {$name}.cpp
end