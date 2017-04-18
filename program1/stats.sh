
#!/bin/bash#!/bin/bash
# 
#
# James Sandoval
# CS 322 Introduction to Operating Systems
# Project #1 - 
# Reference: http://ss64.com/bash/

FILENAME="$2"
#Create a file buffer to hold contents

FAILURE=1
SUCCESS=0

# Function to display correct usage, and ouput to stderr
usage  (){
	echo 
	echo "$0 {-rows|-cols} [filename]" 1>&2
	echo
	exit $FAILURE
}

# Obtain Column/row and variable numbers
fileData (){

	# Count the number of rows and remove spaces and filename
	row_count=$(wc -l $FILENAME | tr -d ' ' | tr -d $FILENAME)

	# Count the Total Number of Numerical Characters in the file
	number_count=$(wc -w < $FILENAME | tr -d ' ')

	# Count the number columns and remove spaces and filename
	let col_count=number_count/row_count
	#echo "the column count is: $col_count"
	#echo "the row count is: $row_count"
	#echo "the number count: $number_count"

}
# Print the Result of Row
printRow (){

				echo
				echo "Average:       Median:"
				for ((i=0;i<${#meanArray[@]};++i))
				do
					printf "%-5s          %s\n" "${meanArray[i]}" "${medianArray[i]}"
				done
}
# Print the Result of Columns
printCol (){

				echo 

				for ((i=0;i<${#meanArray[@]};++i))
				do
					rowMeanPrint+="${meanArray[i]}\t"
					rowMedianPrint+="${medianArray[i]}\t"
				done 
				echo "Averages:"
				echo -e "$rowMeanPrint"
				echo "Median:"
				echo -e "$rowMedianPrint"
				echo

}


# Function to handle file read errors, and ouput to stderr
fileReadError  (){
	echo 
	echo "File does not exist or is unreadable." 1>&2
	echo
	exit $FAILURE
}
# Print the file 
printFile (){
		echo  
		echo
	while read -r line || [[ -n "$line" ]]
		do

    	name="$line"
    	echo "$name"

		done < "$1"

		}

#Transpose Rows to Columns
columnConvert () {

	#remove old test_output
	rm test_output
	touch test_output

	counter=0

	for x in `cat $1`

		do
				stringArray[$counter]+="$x\t" 
				value=${stringArray[$counter]}

				let counter=$counter+1

			if [[ $counter == $col_count ]]; then

				value=${stringArray[$counter]}

				counter=0


			fi
	
		done

		for i in "${stringArray[@]}"

		do
			echo -e $i >> test_output

		done

		process test_output
		rm test_output

}

#Compute Input Values
process (){

	total=0
	counter=0
    while read i; do
    	total=0
  		for num in $i; do
    			total=$(($total + $num))

   				mean=$(($total/col_count))

   				sorted=$(echo $(printf "%s\n" $i | sort -n))

   				# Process Median
   				if [[ $(( $col_count % 2 )) != 0 ]]; then
   					# need to make a comparison here

   					let acompare=col_count/2
   					let bcompare=col_count/2+1

   					if [[ $(echo $sorted | cut -d' ' -f$bcompare,$bcompare) > $(echo $sorted | cut -d' ' -f$acompare,$acompare) ]]; then

   						median=$(echo $sorted | cut -d' ' -f$bcompare,$bcompare)

   					else

   						median=$(echo $sorted | cut -d' ' -f$acompare,$acompare)
   					fi	

 				else
 					#echo "this should be working"
 					let acompare=col_count/2
 					median=$(echo $sorted | cut -d' ' -f$acompare,$acompare)

   				fi
   				
  		done

   				meanArray[$counter]=$mean
    			medianArray[$counter]=$median
    			let counter=counter+1

    #Values printed for troubleshooting	
  	#echo -e "  -->    sum: $total          Mean: $mean           Median: $median         Sorted: $sorted" 
	done < $1
}

# If arguments are > 2, call usage function
if [[ $# > 2 || $1 != -c* && $1 != -r* ]]; then 
	usage 
fi


#evaluate the 2nd argument ->if $2=NULL program should allow user to stdin
if [[ -z "$2" ]]; then


	echo -n "Please Input a file"
	echo 
	usage

# if a file is present notify the user
elif [[ -f $FILENAME ]]; then

	echo "File Processing...."
	#printFile $FILENAME
		
else
	fileReadError 
fi

# Evaluates the first argument to be -c or -c*
if [[ $1 == -c* ]]; then

	fileData

    columnConvert $FILENAME
    printCol
fi

# Evaluates the first argument to be -r or -r*
if [[ $1 == -r* ]]; then

	fileData

	process $FILENAME
	printRow

fi


