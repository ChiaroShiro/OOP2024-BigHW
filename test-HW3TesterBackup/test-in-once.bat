@echo off

echo test-part-1:

test_args_analyse_tools-1-demo > result-demo.txt
test_args_analyse_tools-1-demo --help >> result-demo.txt
test_args_analyse_tools-1-demo --bool >> result-demo.txt

test_args_analyse_tools-1 > result.txt
test_args_analyse_tools-1 --help >> result.txt
test_args_analyse_tools-1 --bool >> result.txt

echo test-part-2:

test_args_analyse_tools-1-demo --intdef 123 >> result-demo.txt
test_args_analyse_tools-1-demo --intdef 123 --intdef 456 >> result-demo.txt
test_args_analyse_tools-1-demo --intdef -12 >> result-demo.txt
test_args_analyse_tools-1-demo --intdef 654321 >> result-demo.txt

test_args_analyse_tools-1 --intdef 123 >> result.txt
test_args_analyse_tools-1 --intdef 123 --intdef 456 >> result.txt
test_args_analyse_tools-1 --intdef -12 >> result.txt
test_args_analyse_tools-1 --intdef 654321 >> result.txt

echo test-part-3:

test_args_analyse_tools-1-demo --interr 123 >> result-demo.txt
test_args_analyse_tools-1-demo --interr 123 --interr 456 >> result-demo.txt
test_args_analyse_tools-1-demo --interr -12 >> result-demo.txt
test_args_analyse_tools-1-demo --interr 654321 >> result-demo.txt

test_args_analyse_tools-1 --interr 123 >> result.txt
test_args_analyse_tools-1 --interr 123 --interr 456 >> result.txt
test_args_analyse_tools-1 --interr -12 >> result.txt
test_args_analyse_tools-1 --interr 654321 >> result.txt

echo test-part-4:

test_args_analyse_tools-1-demo --intsetdef 11 >> result-demo.txt
test_args_analyse_tools-1-demo --intsetdef 12 >> result-demo.txt
test_args_analyse_tools-1-demo --intsetdef 11 --intsetdef 22 >> result-demo.txt

test_args_analyse_tools-1 --intsetdef 11 >> result.txt
test_args_analyse_tools-1 --intsetdef 12 >> result.txt
test_args_analyse_tools-1 --intsetdef 11 --intsetdef 22 >> result.txt

echo test-part-5:

test_args_analyse_tools-1-demo --intseterr 11 >> result-demo.txt
test_args_analyse_tools-1-demo --intseterr 12 >> result-demo.txt
test_args_analyse_tools-1-demo --intseterr 11 --intseterr 22 >> result-demo.txt

test_args_analyse_tools-1 --intseterr 11 >> result.txt
test_args_analyse_tools-1 --intseterr 12 >> result.txt
test_args_analyse_tools-1 --intseterr 11 --intseterr 22 >> result.txt

echo test-part-6:

test_args_analyse_tools-1-demo --doubledef 2.34 >> result-demo.txt
test_args_analyse_tools-1-demo --doubledef 2.34 --doubledef 4.32 >> result-demo.txt
test_args_analyse_tools-1-demo --doubledef -3 >> result-demo.txt
test_args_analyse_tools-1-demo --doubledef 101.234 >> result-demo.txt

test_args_analyse_tools-1 --doubledef 2.34 >> result.txt
test_args_analyse_tools-1 --doubledef 2.34 --doubledef 4.32 >> result.txt
test_args_analyse_tools-1 --doubledef -3 >> result.txt
test_args_analyse_tools-1 --doubledef 101.234 >> result.txt

echo test-part-7:

test_args_analyse_tools-1-demo --doubleerr 2.34 >> result-demo.txt
test_args_analyse_tools-1-demo --doubleerr 2.34 --doubledef 4.32 >> result-demo.txt
test_args_analyse_tools-1-demo --doubleerr -3 >> result-demo.txt
test_args_analyse_tools-1-demo --doubleerr 101.234 >> result-demo.txt
test_args_analyse_tools-1-demo --doublesetdef 12.3 >> result-demo.txt
test_args_analyse_tools-1-demo --doublesetdef 5.6 >> result-demo.txt
test_args_analyse_tools-1-demo --doublesetdef 1.1 --doublesetdef 2.2 >> result-demo.txt

test_args_analyse_tools-1 --doubleerr 2.34 >> result.txt
test_args_analyse_tools-1 --doubleerr 2.34 --doubledef 4.32 >> result.txt
test_args_analyse_tools-1 --doubleerr -3 >> result.txt
test_args_analyse_tools-1 --doubleerr 101.234 >> result.txt
test_args_analyse_tools-1 --doublesetdef 12.3 >> result.txt
test_args_analyse_tools-1 --doublesetdef 5.6 >> result.txt
test_args_analyse_tools-1 --doublesetdef 1.1 --doublesetdef 2.2 >> result.txt

echo test-part-8:

test_args_analyse_tools-1-demo --doubleseterr 12.3 >> result-demo.txt
test_args_analyse_tools-1-demo --doubleseterr 5.6 >> result-demo.txt
test_args_analyse_tools-1-demo --doubleseterr 1.1 --doubleseterr 2.2 >> result-demo.txt

test_args_analyse_tools-1 --doubleseterr 12.3 >> result.txt
test_args_analyse_tools-1 --doubleseterr 5.6 >> result.txt
test_args_analyse_tools-1 --doubleseterr 1.1 --doubleseterr 2.2 >> result.txt

echo test-part-9:

test_args_analyse_tools-1-demo --str1 hello >> result-demo.txt
test_args_analyse_tools-1-demo --str1 hello --str1 horse >> result-demo.txt

test_args_analyse_tools-1-demo --str2 hello >> result-demo.txt
test_args_analyse_tools-1-demo --str2 hello --str2 horse >> result-demo.txt

test_args_analyse_tools-1 --str1 hello >> result.txt
test_args_analyse_tools-1 --str1 hello --str1 horse >> result.txt

test_args_analyse_tools-1 --str2 hello >> result.txt
test_args_analyse_tools-1 --str2 hello --str2 horse >> result.txt

echo test-part-10:

test_args_analyse_tools-1-demo --strsetdef md5 >> result-demo.txt
test_args_analyse_tools-1-demo --strsetdef md4 >> result-demo.txt
test_args_analyse_tools-1-demo --strsetdef md5 --strsetdef sha1 >> result-demo.txt

test_args_analyse_tools-1 --strsetdef md5 >> result.txt
test_args_analyse_tools-1 --strsetdef md4 >> result.txt
test_args_analyse_tools-1 --strsetdef md5 --strsetdef sha1 >> result.txt

echo test-part-11:

test_args_analyse_tools-1-demo --strseterr md5 >> result-demo.txt
test_args_analyse_tools-1-demo --strseterr md4 >> result-demo.txt
test_args_analyse_tools-1-demo --strseterr md5 --strseterr sha1 >> result-demo.txt

test_args_analyse_tools-1 --strseterr md5 >> result.txt
test_args_analyse_tools-1 --strseterr md4 >> result.txt
test_args_analyse_tools-1 --strseterr md5 --strseterr sha1 >> result.txt

echo test-part-12:

test_args_analyse_tools-1-demo --ipdef 1.1.1.1 >> result-demo.txt
test_args_analyse_tools-1-demo --ipdef 1.1.1.1234 >> result-demo.txt
test_args_analyse_tools-1-demo --ipdef 1.1.1.1 --ipdef 2.2.2.2 >> result-demo.txt

test_args_analyse_tools-1 --ipdef 1.1.1.1 >> result.txt
test_args_analyse_tools-1 --ipdef 1.1.1.1234 >> result.txt
test_args_analyse_tools-1 --ipdef 1.1.1.1 --ipdef 2.2.2.2 >> result.txt

echo test-part-13:

test_args_analyse_tools-1-demo --iperr 1.1.1.1 >> result-demo.txt
test_args_analyse_tools-1-demo --iperr 1.1.1.1234 >> result-demo.txt
test_args_analyse_tools-1-demo --iperr 1.1.1.1 --iperr 2.2.2.2 >> result-demo.txt

test_args_analyse_tools-1 --iperr 1.1.1.1 >> result.txt
test_args_analyse_tools-1 --iperr 1.1.1.1234 >> result.txt
test_args_analyse_tools-1 --iperr 1.1.1.1 --iperr 2.2.2.2 >> result.txt


echo test-part-14:

test_args_analyse_tools-0-demo >> result-demo.txt
test_args_analyse_tools-0-demo --help >> result-demo.txt
test_args_analyse_tools-0-demo --bool >> result-demo.txt

test_args_analyse_tools-0 >> result.txt
test_args_analyse_tools-0 --help >> result.txt
test_args_analyse_tools-0 --bool >> result.txt

echo test-part-15:

test_args_analyse_tools-0-demo --intdef 123 >> result-demo.txt
test_args_analyse_tools-0-demo --intdef 123 --intdef 456 >> result-demo.txt
test_args_analyse_tools-0-demo --intdef -12 >> result-demo.txt
test_args_analyse_tools-0-demo --intdef 654321 >> result-demo.txt

test_args_analyse_tools-0 --intdef 123 >> result.txt
test_args_analyse_tools-0 --intdef 123 --intdef 456 >> result.txt
test_args_analyse_tools-0 --intdef -12 >> result.txt
test_args_analyse_tools-0 --intdef 654321 >> result.txt

echo test-part-16:

test_args_analyse_tools-0-demo --interr 123 >> result-demo.txt
test_args_analyse_tools-0-demo --interr 123 --interr 456 >> result-demo.txt
test_args_analyse_tools-0-demo --interr -12 >> result-demo.txt
test_args_analyse_tools-0-demo --interr 654321 >> result-demo.txt

test_args_analyse_tools-0 --interr 123 >> result.txt
test_args_analyse_tools-0 --interr 123 --interr 456 >> result.txt
test_args_analyse_tools-0 --interr -12 >> result.txt
test_args_analyse_tools-0 --interr 654321 >> result.txt

echo test-part-17:

test_args_analyse_tools-0-demo --intsetdef 11 >> result-demo.txt
test_args_analyse_tools-0-demo --intsetdef 12 >> result-demo.txt
test_args_analyse_tools-0-demo --intsetdef 11 --intsetdef 22 >> result-demo.txt

test_args_analyse_tools-0 --intsetdef 11 >> result.txt
test_args_analyse_tools-0 --intsetdef 12 >> result.txt
test_args_analyse_tools-0 --intsetdef 11 --intsetdef 22 >> result.txt

echo test-part-18:

test_args_analyse_tools-0-demo --intseterr 11 >> result-demo.txt
test_args_analyse_tools-0-demo --intseterr 12 >> result-demo.txt
test_args_analyse_tools-0-demo --intseterr 11 --intseterr 22 >> result-demo.txt

test_args_analyse_tools-0 --intseterr 11 >> result.txt
test_args_analyse_tools-0 --intseterr 12 >> result.txt
test_args_analyse_tools-0 --intseterr 11 --intseterr 22 >> result.txt

echo test-part-19:

test_args_analyse_tools-0-demo --doubledef 2.34 >> result-demo.txt
test_args_analyse_tools-0-demo --doubledef 2.34 --doubledef 4.32 >> result-demo.txt
test_args_analyse_tools-0-demo --doubledef -3 >> result-demo.txt
test_args_analyse_tools-0-demo --doubledef 101.234 >> result-demo.txt

test_args_analyse_tools-0 --doubledef 2.34 >> result.txt
test_args_analyse_tools-0 --doubledef 2.34 --doubledef 4.32 >> result.txt
test_args_analyse_tools-0 --doubledef -3 >> result.txt
test_args_analyse_tools-0 --doubledef 101.234 >> result.txt

echo test-part-20:

test_args_analyse_tools-0-demo --doubleerr 2.34 >> result-demo.txt
test_args_analyse_tools-0-demo --doubleerr 2.34 --doubledef 4.32 >> result-demo.txt
test_args_analyse_tools-0-demo --doubleerr -3 >> result-demo.txt
test_args_analyse_tools-0-demo --doubleerr 101.234 >> result-demo.txt
test_args_analyse_tools-0-demo --doublesetdef 12.3 >> result-demo.txt
test_args_analyse_tools-0-demo --doublesetdef 5.6 >> result-demo.txt
test_args_analyse_tools-0-demo --doublesetdef 1.1 --doublesetdef 2.2 >> result-demo.txt

test_args_analyse_tools-0 --doubleerr 2.34 >> result.txt
test_args_analyse_tools-0 --doubleerr 2.34 --doubledef 4.32 >> result.txt
test_args_analyse_tools-0 --doubleerr -3 >> result.txt
test_args_analyse_tools-0 --doubleerr 101.234 >> result.txt
test_args_analyse_tools-0 --doublesetdef 12.3 >> result.txt
test_args_analyse_tools-0 --doublesetdef 5.6 >> result.txt
test_args_analyse_tools-0 --doublesetdef 1.1 --doublesetdef 2.2 >> result.txt

echo test-part-21:

test_args_analyse_tools-0-demo --doubleseterr 12.3 >> result-demo.txt
test_args_analyse_tools-0-demo --doubleseterr 5.6 >> result-demo.txt
test_args_analyse_tools-0-demo --doubleseterr 1.1 --doubleseterr 2.2 >> result-demo.txt

test_args_analyse_tools-0 --doubleseterr 12.3 >> result.txt
test_args_analyse_tools-0 --doubleseterr 5.6 >> result.txt
test_args_analyse_tools-0 --doubleseterr 1.1 --doubleseterr 2.2 >> result.txt

echo test-part-22:

test_args_analyse_tools-0-demo --str1 hello >> result-demo.txt
test_args_analyse_tools-0-demo --str1 hello --str1 horse >> result-demo.txt

test_args_analyse_tools-0-demo --str2 hello >> result-demo.txt
test_args_analyse_tools-0-demo --str2 hello --str2 horse >> result-demo.txt

test_args_analyse_tools-0 --str1 hello >> result.txt
test_args_analyse_tools-0 --str1 hello --str1 horse >> result.txt

test_args_analyse_tools-0 --str2 hello >> result.txt
test_args_analyse_tools-0 --str2 hello --str2 horse >> result.txt

echo test-part-23:

test_args_analyse_tools-0-demo --strsetdef md5 >> result-demo.txt
test_args_analyse_tools-0-demo --strsetdef md4 >> result-demo.txt
test_args_analyse_tools-0-demo --strsetdef md5 --strsetdef sha1 >> result-demo.txt

test_args_analyse_tools-0 --strsetdef md5 >> result.txt
test_args_analyse_tools-0 --strsetdef md4 >> result.txt
test_args_analyse_tools-0 --strsetdef md5 --strsetdef sha1 >> result.txt

echo test-part-24:

test_args_analyse_tools-0-demo --strseterr md5 >> result-demo.txt
test_args_analyse_tools-0-demo --strseterr md4 >> result-demo.txt
test_args_analyse_tools-0-demo --strseterr md5 --strseterr sha1 >> result-demo.txt

test_args_analyse_tools-0 --strseterr md5 >> result.txt
test_args_analyse_tools-0 --strseterr md4 >> result.txt
test_args_analyse_tools-0 --strseterr md5 --strseterr sha1 >> result.txt

echo test-part-25:

test_args_analyse_tools-0-demo --ipdef 1.1.1.1 >> result-demo.txt
test_args_analyse_tools-0-demo --ipdef 1.1.1.1234 >> result-demo.txt
test_args_analyse_tools-0-demo --ipdef 1.1.1.1 --ipdef 2.2.2.2 >> result-demo.txt

test_args_analyse_tools-0 --ipdef 1.1.1.1 >> result.txt
test_args_analyse_tools-0 --ipdef 1.1.1.1234 >> result.txt
test_args_analyse_tools-0 --ipdef 1.1.1.1 --ipdef 2.2.2.2 >> result.txt

echo test-part-26:

test_args_analyse_tools-0-demo --iperr 1.1.1.1 >> result-demo.txt
test_args_analyse_tools-0-demo --iperr 1.1.1.1234 >> result-demo.txt
test_args_analyse_tools-0-demo --iperr 1.1.1.1 --iperr 2.2.2.2 >> result-demo.txt

test_args_analyse_tools-0 --iperr 1.1.1.1 >> result.txt
test_args_analyse_tools-0 --iperr 1.1.1.1234 >> result.txt
test_args_analyse_tools-0 --iperr 1.1.1.1 --iperr 2.2.2.2 >> result.txt


echo test-part-27:

test_args_analyse_tools-0-demo --help --intdef 654321 --interr 123 --intsetdef 12 --intseterr 11 --doubledef 101.234 --doubleerr 2.34 --doublesetdef 12.3 --doubleseterr 12.3 --str1 Hello, --str2 "Tong Ji" --strsetdef sha0 --strseterr md5 --ipdef 1.1.1.1234 --iperr 10.80.42.245 >> result-demo.txt

test_args_analyse_tools-0 --help --intdef 654321 --interr 123 --intsetdef 12 --intseterr 11 --doubledef 101.234 --doubleerr 2.34 --doublesetdef 12.3 --doubleseterr 12.3 --str1 Hello, --str2 "Tong Ji" --strsetdef sha0 --strseterr md5 --ipdef 1.1.1.1234 --iperr 10.80.42.245 >> result.txt

echo test-part-28:

test_args_analyse_tools-0-demo --help --intdef 654321 --interr 123 --intsetdef 12 --intseterr 11 --doubledef 101.234 --doubleerr 2.34 --doublesetdef 12.3 --doubleseterr 12.3 --str1 Hello, --str2 "Tong Ji" --strsetdef sha0 --strseterr md5 --ipdef 1.1.1.1234 --iperr 10.80.42.245 extra >> result-demo.txt

test_args_analyse_tools-0 --help --intdef 654321 --interr 123 --intsetdef 12 --intseterr 11 --doubledef 101.234 --doubleerr 2.34 --doublesetdef 12.3 --doubleseterr 12.3 --str1 Hello, --str2 "Tong Ji" --strsetdef sha0 --strseterr md5 --ipdef 1.1.1.1234 --iperr 10.80.42.245 extra >> result.txt


echo test-part-29:

test_args_analyse_tools-1-demo --help --intdef 654321 --interr 123 --intsetdef 12 --intseterr 11 --doubledef 101.234 --doubleerr 2.34 --doublesetdef 12.3 --doubleseterr 12.3 --str1 Hello, --str2 "Tong Ji" --strsetdef sha0 --strseterr md5 --ipdef 1.1.1.1234 --iperr 10.80.42.245 >> result-demo.txt

test_args_analyse_tools-1 --help --intdef 654321 --interr 123 --intsetdef 12 --intseterr 11 --doubledef 101.234 --doubleerr 2.34 --doublesetdef 12.3 --doubleseterr 12.3 --str1 Hello, --str2 "Tong Ji" --strsetdef sha0 --strseterr md5 --ipdef 1.1.1.1234 --iperr 10.80.42.245 >> result.txt

echo test-part-30:

test_args_analyse_tools-1-demo --help --intdef 654321 --interr 123 --intsetdef 12 --intseterr 11 --doubledef 101.234 --doubleerr 2.34 --doublesetdef 12.3 --doubleseterr 12.3 --str1 Hello, --str2 "Tong Ji" --strsetdef sha0 --strseterr md5 --ipdef 1.1.1.1234 --iperr 10.80.42.245 extra1 extra2 >> result-demo.txt

test_args_analyse_tools-1 --help --intdef 654321 --interr 123 --intsetdef 12 --intseterr 11 --doubledef 101.234 --doubleerr 2.34 --doublesetdef 12.3 --doubleseterr 12.3 --str1 Hello, --str2 "Tong Ji" --strsetdef sha0 --strseterr md5 --ipdef 1.1.1.1234 --iperr 10.80.42.245 extra1 extra2 >> result.txt


comp /m result-demo.txt result.txt

PAUSE