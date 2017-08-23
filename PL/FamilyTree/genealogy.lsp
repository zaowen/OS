#|
 |Program: Program 3: Lisp database
 |Author: Zachary Owen
 |
 |Instructor: Dr.John Weiss
 |Class: Programming Languages CSC 461 
 |Date: December 6 2015
 |
 |A Lisp program that reads in a file of family relationships, builds a database, and then
 |allows queries to that database. Queries include: parents, mothers, fathers, children, sons,
 |daughters, siblings, sisters, brothers, grandparents, grandfathers, grandmothers, grandchildren,
 |grandsons, granddaughters, uncles, aunts, nieces, nephews, cousins, male-cousins, femalecousins,
 |ancestors, male-ancestors, female-ancestors, descendants, male-descendants, and
 |female-descendants
 |
 |Usage: clisp -repl genealogy.lsp database.dat
 |Bugs: None Known
 |
 |#
;;; FileIo Function (mostly) written by Dr. Weiss
(DEFUN FILEIO ( FILENAME )
  "(FILEIO FILENAME: OPEN AN INPUT FILE AND READ DATA"
					;CHECK USAGE
  (WHEN (NULL FILENAME)
	( PROGN (FORMAT T "PROPER USAGE: genealogy.lsp database.dat" ) (QUIT)  )
	)

  (SETF FIN (OPEN FILENAME :IF-DOES-NOT-EXIST NIL) )   ; OPEN FILE, RETURNING NIL ON ERROR
  (WHEN (NULL FIN) (PROGN (FORMAT T "ERROR: CANNOT OPEN FILE ~A" FILENAME) (EXIT) )  ) 

  (LET ( DATA (MYLIST (LIST NIL ) ) )
       (DO ( (DATA (READ FIN NIL) (READ FIN NIL) ) )          ; READ ENTIRE FILE, RETURNING NIL AT EOF
	   ( (NULL DATA) (CLOSE FIN) )                       ; EXIT WHEN FILE IS READ
	   ;; puts data fields into data the person structure
	   (SETQ DATA ( MAKE-PERSON :NAME (CAR DATA)
				    :GENDER (CAR ( CDR DATA ) )
				    :PARENTS (CAR ( CDR ( CDR DATA ) ) ) 
				    :CHILDREN (CAR ( CDR ( CDR ( CDR DATA ) ) ) )
				    )
		 )
	   (SETF MYLIST(CONS DATA MYLIST ) ) ;put data into mylist
	   
	   )
       (remove nil MYLIST) ;Returns mylist into database and removes final nil that was causing problems
       )
  )

;;;Returns Parents of the name in a list or nil if they arent in the database 
(DEFUN PARENTS ( PERSON )
  ;;look for person return struct if yes nil if no
  (LET ( ( ENTRY ( FINDNL PERSON ) ) )
       ;;either nil or return the name of the person
       (COND
	( (NULL ENTRY)  NIL ) 
	( T (PERSON-PARENTS  ENTRY  ) )
	)
       )
  )

;;;This function takes a name and a copy of the database we can car/cdr over
(DEFUN FINDDOOD ( MYNAME MYLIST )
  (COND
   ( ( NULL MYLIST) NIL  )  ; if list is empty return nil
   ( (EQUAL (PERSON-NAME ( CAR MYLIST ) ) MYNAME  ) (CAR MYLIST)  ) ; if the car entry matches returnit
   (T ( FINDDOOD MYNAME (CDR MYLIST) ) ) ;otherwise recurs on cdr
   )
  )

;;Easier to call this function rather than FINDDOOD since FINDN(o)L(ist) and much easier to mapcar on lists of names
(DEFUN FINDNL ( PERSON )
  (FINDDOOD PERSON *DATABASE*)  ; call actual function wiht *database*
  )

;;Found Outline for this on GitHub
;;Function for concatenating items in lists of lists
;;being passed a list the final condition will always be true
;; the first time around and then we call and concatenate and map
(DEFUN FLATTEN ( LIST)
  (COND
   ((NULL LIST) NIL ) ; dont worry if nil
   ( (ATOM LIST ) `( , LIST) ) ; again dont worry if atom
   ( T (MAPCAN #'FLATTEN LIST) ) ;map function on whole list
   ) 
  )

;;Mothers are GirlFiltered lists of parents
(DEFUN MOTHERS ( PERSON )
  (LET ( ( ENTRY (PARENTS PERSON ) ) )
       ( GF ENTRY )
       )
  )

;;Fathers are MaleFiltered lists of parents
(DEFUN FATHERS ( PERSON )
  (LET ( ( ENTRY (PARENTS PERSON ) ) )
       ( MF  ENTRY )
       )
  )

;;Children are a field in the database find and return
(DEFUN CHILDREN ( PERSON )
  (LET ( ( ENTRY  ( FINDNL PERSON) ) ) ; find entry
       (COND
	( (NULL ENTRY) NIL ) ; nil if its not there
	( T (PERSON-CHILDREN  ENTRY  ) ) ; list if it is
	)
       )
  )

;;Sons are MF() lists of children
(DEFUN SONS ( PERSON )
  (LET ( ( ENTRY (CHILDREN PERSON ) ) )
       ( MF ENTRY )
       )
  )

;;Daughters are GF() lists of children
(DEFUN DAUGHTERS ( PERSON )
  (LET ( ( ENTRY (CHILDREN PERSON ) ) )
       ( GF  ENTRY )
       )
  )
#|
Finds Siblings by finding parents and then maping
the children function flattening it removing duplicates
and then removing the person in question
Note: if there are duplicate names they will be removed 
and if a person has siblings with identical names 
that person has odd parents
|#
(DEFUN SIBLINGS ( PERSON )
  (LET ( (ENTRY (PARENTS PERSON ) ) )
       (REMOVE PERSON  (REMOVE-DUPLICATES (FLATTEN (MAPCAR #'CHILDREN ENTRY) ) ) ) 
       )
  )

#|
Uses sibling function and then Girl filters it
|#
(DEFUN SISTERS (  PERSON )
  (LET ( ( ENTRY ( SIBLINGS PERSON) ) )
       (GF ENTRY)
       )
  )

#|
Uses sibling function and then male filters it
|#
(DEFUN BROTHERS ( PERSON )
  (LET ( ( ENTRY ( SIBLINGS PERSON) ) )
       (MF ENTRY )
       )
  )

#|
Maps the parent function on the parent function 
then flattens and removes duplicates just in case
|#
(DEFUN GRANDPARENTS ( PERSON )
  (LET ( ( ENTRY ( PARENTS PERSON ) ) )
       (REMOVE-DUPLICATES (FLATTEN (MAPCAR #'PARENTS  ENTRY ) ) )
       )
  )
#|
Calls Grandparents and then Male Filters it
|#
(DEFUN GRANDFATHERS ( PERSON )
  (LET ( ( ENTRY ( GRANDPARENTS PERSON ) ) ) 
       (MF ENTRY )
       )
  )

#|
Calls Grandparents and then Girl Filters it
|#
(DEFUN GRANDMOTHERS ( PERSON )
  (LET ( ( ENTRY ( GRANDPARENTS PERSON ) ) )
       (GF ENTRY ) 
       )
  )

#|
Maps the Children function on the return from the 
Children function and flattens that
Note: Possible edge case of duplicates so they are removed
|#
(DEFUN GRANDCHILDREN ( PERSON )
  (LET ( ( ENTRY ( CHILDREN PERSON ) ) )
       (REMOVE-DUPLICATES (FLATTEN (MAPCAR #'CHILDREN  ENTRY ) ) )
       )
  )
#|
Calls Grandchildren and Male filters it
|#
(DEFUN GRANDSONS ( PERSON )
  (LET ( ( ENTRY ( GRANDCHILDREN PERSON ) ) )
       (MF ENTRY )
       )
  )
#|
These Functions really explain themselves
|#
(DEFUN GRANDDAUGHTERS ( PERSON )
  (LET ( ( ENTRY ( GRANDCHILDREN PERSON ) ) )
       (GF  ENTRY )
       )
  )

#|
A(unts)ANDU(ncles) Not a required function 
but is helpful for uncles, aunts and cousin functions
|#
(DEFUN AANDU ( PERSON )
  (LET (( ENTRY ( PARENTS PERSON ) ))
       (FLATTEN (MAPCAR #'SIBLINGS  ENTRY ) )
       )
  )

#|
Calls ANNDU and male filters the results
|#
(DEFUN UNCLES ( PERSON )
  (LET (( ENTRY ( AANDU PERSON ) ))
       (MF ENTRY)
       )
  )

#|
Calls ANNDU and Girl Filters the Result
|#
(DEFUN AUNTS ( PERSON )
  (LET (( ENTRY ( AANDU PERSON ) ))
       (GF ENTRY )
       )
  )

#|
Calls Sibling function and maps Children on its results
Female filters it
|#
(DEFUN NIECES ( PERSON )
  (LET (( ENTRY ( SIBLINGS PERSON) ))
       (GF (FLATTEN (MAPCAR #'CHILDREN ENTRY )))
       )
  )

#|
Calls Sibling maps children and Male Filters
|#
(DEFUN NEPHEWS ( PERSON )
  (LET (( ENTRY ( SIBLINGS PERSON) ))
       (MF (FLATTEN (MAPCAR #'CHILDREN ENTRY )))
       )
  )

#|
Finds Aunts and Uncles and and returns their children flattened
Not sure if duplicates are possible but safeguarded in case
|#
(DEFUN COUSINS ( PERSON )
  (LET (( ENTRY ( AANDU PERSON ) ))
       (REMOVE-DUPLICATES (FLATTEN ( MAPCAR #'CHILDREN ENTRY ) ))
       )
  )

#|
Uses Cousin function and Male Filters it
Suffers same downsides as Cousin func
|#
(DEFUN MALE-COUSINS ( PERSON )
  (LET (( ENTRY ( COUSINS PERSON ) ))
       (MF ENTRY )
       )
  )

#|
Uses Cousin function and Girl Filters it
Suffers same downsides as Cousin func
|#
(DEFUN FEMALE-COUSINS ( PERSON )
  (LET (( ENTRY ( COUSINS PERSON ) ))
       (GF ENTRY)
       )
  )
#|
Recursively calls parents and maps parent on those parents
Nil cases are taken care of in Parent Function
Duplicates are removed for the sake of edgecases
|# 
(DEFUN ANCESTORS ( PERSON )
  (LET (( ENTRY ( PARENTS PERSON ) ))
       (REMOVE-DUPLICATES ( FLATTEN ( LIST ENTRY (MAPCAR #'ANCESTORS ENTRY) ) ) ) 
       )
  )

#|
Finds ancestors and Male filters them
|#
(DEFUN MALE-ANCESTORS ( PERSON )
  (LET (( ENTRY ( ANCESTORS PERSON) ))
       (MF ENTRY )
       )
  ) 

#|
Finds ancestors and Girl filters them
|#
(DEFUN FEMALE-ANCESTORS ( PERSON )
  (LET (( ENTRY ( ANCESTORS PERSON) ))
       (GF ENTRY )
       )
  )

#|
Recursively calls children and maps children on those 
children Nil cases are taken care of in children Function
Duplicates are removed for the sake of edgecases 
More likely than in ancestor function
|# 
(DEFUN DESCENDANTS ( PERSON )
  (LET (( ENTRY ( CHILDREN PERSON ) ))
       (REMOVE-DUPLICATES ( FLATTEN ( LIST ENTRY (MAPCAR #'DESCENDANTS ENTRY) ) ) ) 
       )
  )

#|
Finds Decentants and Male filters them
|#
(DEFUN MALE-DESCENDANTS ( PERSON )
  (LET (( ENTRY ( DESCENDANTS PERSON) ))
       (MF ENTRY )
       )
  )

#|
Finds Decentants and Girl filters them
|#
(DEFUN FEMALE-DESCENDANTS ( PERSON )
  (LET (( ENTRY ( DESCENDANTS PERSON) ))
       (GF ENTRY )
       )
  )

#|
Generic filter for Gender so the MF and GF functions
Can be simpler and easier.
Recuses on Car/Cdr removing items that do not match the 
GENDER argument. 
NEEDS TO BE 'MALE or 'FEMALE
|#
(DEFUN FILTER ( MYLIST  GENDER)
  (COND
   ( ( NULL MYLIST) NIL )
   (( EQUAL (PERSON-GENDER ( CAR MYLIST) ) GENDER) (CONS ( PERSON-NAME (CAR MYLIST )) ( FILTER (CDR MYLIST ) GENDER ) ) )
   ( T ( FILTER (CDR MYLIST ) GENDER) )
   )
  )

#|
Calls the Generic Filter and maps the FINDNL function
on MYLIST since the Filter acesses the struct directly
Had to be done somewhere, here seemed most readable
Sends male gender to FILTER
|#
(DEFUN MF (MYLIST)
  (FILTER (MAPCAR #'FINDNL MYLIST) 'MALE )   
  )

#|
Calls the Generic Filter and maps the FINDNL function
on MYLIST since the Filter acesses the struct directly
Had to be done somewhere, here seemed most readable
Sends female gender to FILTER
|#
(DEFUN GF (MYLIST)
  (FILTER (MAPCAR #'FINDNL MYLIST) 'FEMALE )   
  )

#|
Structure that holds database entries
|#
( DEFSTRUCT PERSON
  NAME
  GENDER
  PARENTS
  CHILDREN
  )

;;Called on module load
(SETF *DATABASE* (FILEIO ( CAR *ARGS*) ) )

