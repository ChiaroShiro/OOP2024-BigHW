#include <bits/stdc++.h>
#include <windows.h>
using namespace std;


const string filenamesOOP[] = {
    "12-b1-1.cpp",
    "12-b1-2.cpp",
    "12-b2.c",
    "12-b3.cpp",
    "12-b5-1.cpp",
    "12-b5-2.cpp",
    "13-b2-1.c",
    "13-b2-2.cpp",
    "13-b2-3.c",
    "13-b2-4.cpp",
    "13-b3-1.c",
    "13-b3-2.cpp",
    "13-b4-1.c",
    "13-b4-2.cpp",
    "13-b5.cpp",
    "13-b6.cpp",
    "13-b8-1.c",
    "13-b8-2.cpp",
    "13-b9-linear_list_sq.h",
    "13-b9-linear_list_sq.c",
    "13-b9-linear_list_sq_main.c",
    "14-b1.cpp",
    "14-b2.cpp",
    "14-b3.cpp",
    "14-b4.cpp",
    "15-b1.cpp",
    "15-b2.cpp",
    "15-b3.cpp",
    "15-b4.c",
    "15-b5.c",
    "15-b6.c",
    "15-b7-1.cpp",
    "15-b7-2.c",
    "15-b8-bmp.h",
    "15-b8-bmp.cpp",
    "16-b2-1.cpp",
    "16-b2-2.hpp",
    "16-b3-1.cpp",
    "16-b3-2.cpp",
    "16-b4.h",
    "16-b4.cpp",
    "16-b5.h",
    "16-b5.cpp",
    "16-b6.h",
    "16-b6.cpp",
    "16-b7.h",
    "16-b7.cpp",
    "90-02-b1.rar",
    "cmd_console_tools.cpp",
    "90-02-b2-pre.rar",
    "90-02-b2.rar",
    "90-02-b3.rar",
    "90-02-bmp.rar",
    "90-02-b4.rar",
    "Report-12-b4.pdf",
    "Report-12-b6.pdf",
    "Report-13-b1.pdf",
    "Report-13-b6.pdf",
    "Report-13-b7.pdf",
    "Report-15-b1.pdf",
    "Report-15-b2.pdf",
    "Report-15-b3.pdf",
    "Report-15-b4.pdf",
    "Report-15-b5.pdf",
    "Report-16-b1.pdf",
    "id_rsa_3072.pub",
    "help.pdf"
};

const string filenamesPI[] = {
    "3-b1-1.c",
    "3-b1-2.cpp",
    "3-b2-1.c",
    "3-b2-2.cpp",
    "3-b3-1.c",
    "3-b3-2.cpp",
    "3-b4-1.c",
    "3-b4-2.cpp",
    "3-b5-1.c",
    "3-b5-2.cpp",
    "3-b6-1.c",
    "3-b6-2.cpp",
    "3-b7-1.c",
    "3-b7-2.cpp",
    "3-b8.cpp",
    "3-b9.c",
    "3-b10-1.cpp",
    "3-b10-2.c",
    "3-b11-1.cpp",
    "3-b11-2.cpp",
    "3-b11-3.cpp",
    "3-b11-4.cpp",
    "3-b11-5.cpp",
    "3-b12-1.cpp",
    "3-b12-2.c",
    "3-b13-1.cpp",
    "3-b13-2.c",
    "4-b1.cpp",
    "4-b2-1.c",
    "4-b2-2.cpp",
    "4-b3.cpp",
    "4-b4-1.c",
    "4-b4-2.cpp",
    "4-b5.cpp",
    "4-b6.c",
    "4-b7.cpp",
    "4-b8.c",
    "4-b9.cpp",
    "4-b10.cpp",
    "4-b11.cpp",
    "4-b12.cpp",
    "4-b13-1.cpp",
    "4-b13-2.c",
    "hw_Chapter4_Part4.rar",
    "4-b18.cpp",
    "4-b19.cpp",
    "5-b1.cpp",
    "5-b2.cpp",
    "5-b3.c",
    "5-b4.c",
    "5-b5.cpp",
    "5-b6-1.c",
    "5-b6-2.cpp",
    "5-b7-main.cpp",
    "5-b8.cpp",
    "5-b9.cpp",
    "5-b10.cpp",
    "5-b11-1.c",
    "5-b11-2.cpp",
    "5-b12-sub.cpp",
    "5-b13.cpp",
    "5-b14.c",
    "5-b15.cpp",
    "5-b16-1.c",
    "5-b16-2.cpp",
    "5-b16-3.cpp",
    "5-b16-4.c",
    "5-b17.cpp",
    "5-b18.cpp",
    "6-b1.cpp",
    "6-b2.cpp",
    "6-b3.cpp",
    "6-b4-sub.cpp",
    "w08-s1.cpp",
    "w08-s2.c",
    "w08-s3.cpp",
    "w11-s1.cpp",
    "w11-s2.c",
    "w13-s1.cpp",
    "w13-s2-1.cpp",
    "w13-s2-2.c",
    "hanoi.h",
    "hanoi_main.cpp",
    "hanoi_menu.cpp",
    "hanoi_multiple_solutions.cpp",
    "90-b2.h",
    "90-b2-main.cpp",
    "90-b2-base.cpp",
    "90-b2-console.cpp",
    "90-b2-tools.cpp",
    "Report-2-b1.pdf",
    "Report-2-b2.pdf",
    "Report-2-b3.pdf",
    "Report-2-b4.pdf",
    "Report-2-b5.pdf",
    "Report-3-b1.pdf",
    "Report-3-b2.pdf",
    "Report-3-b3.pdf",
    "Report-3-b4.pdf",
    "Report-3-b5.pdf",
    "Report-3-b6.pdf",
    "Report-4-b1.pdf",
    "Report-vs2022-debug.pdf",
    "Report-4-b2.pdf",
    "Report-4-b3.pdf",
    "Report-4-b4.pdf",
    "Report-5-b1.pdf",
    "Report-5-b2.pdf",
    "Report-hanoi.pdf",
    "Report-5-b5.pdf",
    "Report-6-b1.pdf",
    "Report-ColorLinez.pdf",
    "99-1.pdf"
};

const string classID[] = {
    "10108001",
    "10108002",
    "5000244001602"
};

const string stuID01[] = {
    "2051231",
    "2152131",
    "2153687",
    "2250889",
    "2251174",
    "2252036",
    "2252109",
    "2350229",
    "2350488",
    "2350489",
    "2350960",
    "2350995",
    "2350996",
    "2350997",
    "2350998",
    "2350999",
    "2351043",
    "2351044",
    "2351106",
    "2351114",
    "2351119",
    "2351136",
    "2351138",
    "2351268",
    "2351274",
    "2351276",
    "2351277",
    "2351285",
    "2351286",
    "2351298",
    "2351395",
    "2351457",
    "2351517",
    "2351518",
    "2351520",
    "2351577",
    "2351579",
    "2351582",
    "2351589",
    "2351593",
    "2351709",
    "2351867",
    "2351884",
    "2351889",
    "2352035",
    "2352045",
    "2352357",
    "2352363",
    "2352462",
    "2352475",
    "2352614",
    "2352615",
    "2352688",
    "2352689",
    "2352743",
    "2352753",
    "2352840",
    "2352841",
    "2352847",
    "2352848",
    "2352975",
    "2352976",
    "2352978",
    "2353113",
    "2353123",
    "2353235",
    "2353244",
    "2353366",
    "2353367",
    "2353396",
    "2353545",
    "2353572",
    "2353578",
    "2353585",
    "2353597",
    "2353598",
    "2353606",
    "2353735",
    "2353738",
    "2353739",
    "2353768",
    "2354180",
    "2354273",
    "2354279",
    "2354366",
    "2354367",
    "2354368"
};

const string stuID02[] = {
    "1951792",
    "1956212",
    "2052137", 
    "2053187",
    "2056004",
    "2150267",
    "2151562",
    "2151848",
    "2152701",
    "2156008",
    "2156015",
    "2250898",
    "2251975",
    "2252102",
    "2253180",
    "2253187",
    "2253334",
    "2253552",
    "2253711",
    "2253767",
    "2350218",
    "2350222",
    "2350231",
    "2350232",
    "2350233",
    "2350491",
    "2350501",
    "2350988",
    "2351125",
    "2351126",
    "2351295",
    "2351406",
    "2351433",
    "2351437",
    "2351449",
    "2351450",
    "2351570",
    "2351573",
    "2351584",
    "2351753",
    "2351871",
    "2351879",
    "2352018",
    "2352027",
    "2352032",
    "2352036",
    "2352189",
    "2352201",
    "2352209",
    "2352314",
    "2352350",
    "2352469",
    "2352596",
    "2352611",
    "2352613",
    "2352618",
    "2352733",
    "2352748",
    "2352831",
    "2352846",
    "2352960",
    "2352962",
    "2352973",
    "2352990",
    "2353232",
    "2353246",
    "2353252",
    "2353259",
    "2353410",
    "2353568",
    "2353573",
    "2353581",
    "2353599",
    "2353732",
    "2353796",
    "2353806",
    "2353915",
    "2353931",
    "2353939",
    "2353941",
    "2354040",
    "2354041",
    "2354086",
    "2354182",
    "2354261",
    "2354352",
    "2356187"
};

const string stuID500[] = {
    "1951663",
    "1953628", 
    "1953812",
    "2050977",
    "2051147",
    "2051231",
    "2053187",
    "2054328",
    "2056173",
    "2152108",
    "2152500",
    "2152778",
    "2153764",
    "2154060",
    "2154062",
    "2154399",
    "2250434",
    "2250615",
    "2250651",
    "2250679",
    "2250690",
    "2250707",
    "2250751",
    "2250755",
    "2250778",
    "2250832",
    "2251415",
    "2251536",
    "2251593",
    "2251746",
    "2251919",
    "2252205",
    "2252427",
    "2252541",
    "2252553",
    "2252612",
    "2252691",
    "2252829",
    "2252908",
    "2252937",
    "2252964",
    "2253021",
    "2253087",
    "2253200",
    "2253434",
    "2253552",
    "2253734",
    "2253745",
    "2253874",
    "2253922",
    "2254173",
    "2350258",
    "2350262",
    "2350280",
    "2350284",
    "2350292",
    "2350338",
    "2350716",
    "2350752",
    "2350795",
    "2350797",
    "2350802",
    "2350959",
    "2350987",
    "2351017",
    "2351073",
    "2351128",
    "2351205",
    "2351232",
    "2351259",
    "2351323",
    "2351324",
    "2351351",
    "2351493",
    "2351571",
    "2351610",
    "2351635",
    "2351686",
    "2351727",
    "2351886",
    "2352030",
    "2352042",
    "2352213",
    "2352216",
    "2352366",
    "2352396",
    "2352483",
    "2352527",
    "2352647",
    "2352663",
    "2352674",
    "2352752",
    "2352834",
    "2352885",
    "2352988",
    "2352994",
    "2353018",
    "2353294",
    "2353328",
    "2353435",
    "2353741",
    "2353750",
    "2353754",
    "2353761",
    "2353773",
    "2353793",
    "2353800",
    "2353821",
    "2353833",
    "2353882",
    "2353921",
    "2353970",
    "2353986",
    "2353987",
    "2354008",
    "2354099",
    "2354104",
    "2354127",
    "2354184",
    "2354218",
    "2354297",
    "2354313",
    "2354323",
    "2354352",
    "2354356",
    "2354448",
    "2356184",
    "2356185",
    "2356190",
    "2356201",
    "2356218"
};

string commandBase[] = {
    "--action base --stu all --cno 10108001 --file all",
    "--action base --stu all --cno 10108002 --file all",
    "--action base --stu all --cno 5000244001602 --file all",

    "--action base --stu 2351871 --cno 10108002 --file all",
    "--action base --stu 2251919 --cno 5000244001602 --file all",
    "--action base --stu 2154062 --cno 5000244001602 --file all",
    "--action base --stu 2351871 --cno 10108002 --file all --week 12",
    "--action base --stu 2351871 --cno 10108002 --file all --chapter 14",
    "--action base --stu 2351871 --cno 10108002 --file all --chapter 16",
    "--action base --stu 2351871 --cno 10108002 --file all --week 8 --chapter 13",
    
    "--action base --stu 2351871 --cno 10108002 --file 15-b8-bmp.cpp",
    "--action base --stu all --cno 10108002 --file 16-b2-2.hpp",
    "--action base --stu all --cno 10108001 --file 16-b5.h",
    "--action base --stu all --cno 10108002 --file 90-02-bmp.rar",
    "--action base --stu all --cno 10108002 --file Report-16-b1.pdf",

    "--action base --stu all --cno 10108001 --file all --week 12",
    "--action base --stu all --cno 10108002 --file all --week 2",
    "--action base --stu all --cno 5000244001602 --file all --week 5",
    "--action base --stu all --cno 5000244001602 --file all --week 5 --chapter 3",
    "--action base --stu all --cno 5000244001602 --file all --week 11",
    "--action base --stu all --cno 10108001 --file all --chapter 15",
    "--action base --stu all --cno 10108002 --file all --chapter 90",
    "--action base --stu all --cno 5000244001602 --file all --chapter 3",
    "--action base --stu all --cno 5000244001602 --file all --chapter 5",
};

string commandFirst[] = {
    "--action firstline --stu all --cno 10108001 --file all",
    "--action firstline --stu all --cno 10108002 --file all",
    "--action firstline --stu all --cno 5000244001602 --file all",

    "--action firstline --stu 2351871 --cno 10108002 --file all",
    "--action firstline --stu 2154062 --cno 5000244001602 --file all",
    "--action firstline --stu 2251919 --cno 5000244001602 --file all",
    "--action firstline --stu 2352988 --cno 5000244001602 --file all",
    "--action firstline --stu 2356218 --cno 5000244001602 --file all",
    "--action firstline --stu 2351871 --cno 10108002 --file all --week 8 --chapter 13",
    "--action firstline --stu all --cno 10108002 --file 16-b2-2.hpp",
    "--action firstline --stu all --cno 10108001 --file 16-b5.h",
    "--action firstline --stu all --cno 5000244001602 --file all --week 5 --chapter 3",
    "--action firstline --stu 2351871 --cno 10108002 --file 15-b8-bmp.cpp",
    "--action firstline --stu all --cno 10108001 --file all --week 12",
    "--action firstline --stu all --cno 10108002 --file all --week 2",
    "--action firstline --stu all --cno 5000244001602 --file all --week 5",
    "--action firstline --stu all --cno 5000244001602 --file all --week 11",
    "--action firstline --stu 2351871 --cno 10108002 --file all --week 12",
    "--action firstline --stu 2351871 --cno 10108002 --file all --week 8",
    "--action firstline --stu all --cno 10108001 --file all --chapter 15",
    "--action firstline --stu all --cno 10108002 --file all --chapter 90",
    "--action firstline --stu all --cno 5000244001602 --file all --chapter 3",
    "--action firstline --stu all --cno 5000244001602 --file all --chapter 5",
    "--action firstline --stu 2351871 --cno 10108002 --file all --chapter 14",
    "--action firstline --stu 2351871 --cno 10108002 --file all --chapter 16",
};

string commandSecond[] = {
    "--action secondline --file 5-b14.c --stu all --cno 5000244001602",

    "--action secondline --file 15-b5.c --stu all --cno 10108001,10108002",
    "--action secondline --file 15-b5.c --stu all --cno 10108002,10108001",
    "--action secondline --file 15-b2.cpp --stu all --cno 10108001,10108002",
    "--action secondline --file 15-b2.cpp --stu all --cno 10108002,10108001",

    "--action secondline --file 15-b5.c --stu all --cno \"10108001, 10108002\"",
    "--action secondline --file 15-b5.c --stu all --cno \"10108002  ,    10108001\"",

    "--action secondline --file 15-b5.c --stu all --cno 10108001",
    "--action secondline --file 15-b5.c --stu all --cno 10108002",
    "--action secondline --file 15-b2.cpp --stu all --cno 10108001",
    "--action secondline --file 15-b2.cpp --stu all --cno 10108002",
};

string commandAdd[] = {
    "--action base --stu all --cno 10108001 --file ",
    "--action base --stu all --cno 10108002 --file ",
    "--action base --stu all --cno 5000244001602 --file ",
};

int main() {
    double baseDemo = 0, baseHw = 0;
    double firstDemo = 0, firstHw = 0;
    double secondDemo = 0, secondHw = 0;
    int baseCount = 0, firstCount = 0, secondCount = 0;

    if(1) {
        cout << "base 测试" << endl;
        for(int i = 0; i < 24; i++) { // base 测试
            cerr << "i = " << i << " " << commandBase[i] << endl;
        
            cerr << "run demo" << endl;
            auto start = chrono::steady_clock::now();
            string cmd = "demo " + commandBase[i] + " > .ans";
            system(cmd.c_str());
            auto end = chrono::steady_clock::now();
            auto demoTime = chrono::duration_cast<chrono::milliseconds>(end - start).count();
            baseDemo += demoTime;
            cerr << "demo运行时间: " << demoTime << "ms" << endl;
            
            cerr << "run hw_check" << endl;
            start = chrono::steady_clock::now();
            cmd = "hw_check " + commandBase[i] + " > .out";
            system(cmd.c_str());
            end = chrono::steady_clock::now();
            auto hwTime = chrono::duration_cast<chrono::milliseconds>(end - start).count();
            baseHw += hwTime;
            cerr << "hw_check运行时间: " << hwTime << "ms" << endl;
            
            cerr << "run tc" << endl;
            cmd = "tc  --file1 .ans --file2 .out --trim right --display normal";
            system(cmd.c_str());
            Sleep(1000);
            baseCount++;
        }
    }
    if(1) {
        cout << "firstline 测试" << endl;
        for(int i = 0; i < 25; i++) { // firstline 测试
            cerr << "i = " << i << " " << commandFirst[i] << endl;
        
            cerr << "run demo" << endl;
            auto start = chrono::steady_clock::now();
            string cmd = "demo " + commandFirst[i] + " > .ans";
            system(cmd.c_str());
            auto end = chrono::steady_clock::now();
            auto demoTime = chrono::duration_cast<chrono::milliseconds>(end - start).count();
            firstDemo += demoTime;
            cerr << "demo运行时间: " << demoTime << "ms" << endl;
            
            cerr << "run hw_check" << endl;
            start = chrono::steady_clock::now();
            cmd = "hw_check " + commandFirst[i] + " > .out";
            system(cmd.c_str());
            end = chrono::steady_clock::now();
            auto hwTime = chrono::duration_cast<chrono::milliseconds>(end - start).count();
            firstHw += hwTime;
            cerr << "hw_check运行时间: " << hwTime << "ms" << endl;
            
            cerr << "run tc" << endl;
            cmd = "tc  --file1 .ans --file2 .out --trim right --display normal";
            system(cmd.c_str());
            Sleep(1000);
            firstCount++;
        }
    }

    if(1) {
        cout << "secondline 测试" << endl;
        for(int i = 0; i < 11; i++) { // secondline 测试
                cerr << "i = " << i << " " << commandSecond[i] << endl;
            cerr << "run demo" << endl;
            auto start = chrono::steady_clock::now();
            string cmd = "demo " + commandSecond[i] + " > .ans";
            system(cmd.c_str());
            auto end = chrono::steady_clock::now();
            auto demoTime = chrono::duration_cast<chrono::milliseconds>(end - start).count();
            secondDemo += demoTime;
            cerr << "demo运行时间: " << demoTime << "ms" << endl;
            
            cerr << "run hw_check" << endl;
            start = chrono::steady_clock::now();
            cmd = "hw_check " + commandSecond[i] + " > .out";
            system(cmd.c_str());
            end = chrono::steady_clock::now();
            auto hwTime = chrono::duration_cast<chrono::milliseconds>(end - start).count();
            secondHw += hwTime;
            cerr << "hw_check运行时间: " << hwTime << "ms" << endl;
            
            cerr << "run tc" << endl;
            cmd = "tc  --file1 .ans --file2 .out --trim right --display normal";
            system(cmd.c_str());
            Sleep(1000);
            secondCount++;
        }
    }

    cout << "\n平均运行时间统计:" << endl;
    if(baseCount > 0) {
        cout << "base模式:" << endl;
        cout << "  demo平均: " << baseDemo/baseCount << "ms" << endl;
        cout << "  hw_check平均: " << baseHw/baseCount << "ms" << endl;
    }
    if(firstCount > 0) {
        cout << "firstline模式:" << endl; 
        cout << "  demo平均: " << firstDemo/firstCount << "ms" << endl;
        cout << "  hw_check平均: " << firstHw/firstCount << "ms" << endl;
    }
    if(secondCount > 0) {
        cout << "secondline模式:" << endl;
        cout << "  demo平均: " << secondDemo/secondCount << "ms" << endl;
        cout << "  hw_check平均: " << secondHw/secondCount << "ms" << endl;
    }
}
