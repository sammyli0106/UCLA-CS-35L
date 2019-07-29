#! /bin/sh
# Find all the words that start with the <td> tag and ends with a <td> tag

   grep -o "<td>.*</td>" |

#  Delete all the html tags that are at the beginning and at the end of the line

   sed 's/<[^>]*>//g' |

   #  Explanation : The expression first search for opening <

   #Then, * refers to match any number of the single character

   #that immediately precedes it which is followed by zero

   #or more characters.

   #The character class [ ] starts with ^ character and looks

   #for characters not in the class which is > character.

   #Finally, the expression searching for the > character.

#  Delete the extra new line characters

   sed '/^$/d' |

#  Remove any English words which is deleting the even number lines
#  to be left with only the Haiwaiian words

   sed -n '0~2p' |
   
#  Transfer all upper case letters with lower case letters
   tr '[:upper:]' '[:lower:]' |

#  Transfer ASCII grave accent to ASCII apostrophe
   tr "\`" "\'" |

#  Search comma then replace with a new line character to separate the
#  two consecutive words

   tr , '\n' |

#  Delete any leading black spaces or tab

   sed -e 's/^[ \t]*//' |

#  Search white space then replace with a new line character
#  to separate the two consecutive words

   sed "s/ /\n/g" |

#  Delete any extra new lines after modification
   sed '/^$/d' |

#  Reject any entries that contain non-Hawaiian letters
   sed "/[^pk\'mnwlhaeiou]/d" |

#  Sort the resulting list of words and remove duplicates of the words

   sort -u
