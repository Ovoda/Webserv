counter=0
while [ $counter -ge -1 ]
do
	curl 127.0.0.1:18001/index.php
	if [ $? != 0 ]; then
   		echo "matched"
		exit
	else
		echo $counter
	fi
	((counter++))
done
