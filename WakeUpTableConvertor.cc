/*
	ReadFile : argv[1] - WakeUpTimes_date.log
	OutputFile : WakeUpTimes_date.html
*/
#include<fstream>
#include<iostream>
#include<string>

using namespace std;

void readFileName(string &ReadName) {
	cout << "Enter the file name to read: ";
	getline(cin, ReadName);
}

void openFile(string &Name, fstream &File) {
	if((Name.find("log") != string::npos) || (Name.find("txt") != string::npos)) File.open(Name.c_str(), ios::in);
	else File.open(Name.c_str(), ios::out | ios::trunc);
	if(!File) cout << "Open " << Name << " failed.\n";
	else cout << "Open " << Name << " successful.\n";
}

int calNum(string &Line) {
	int ApkNum = 0;
	Line.erase(0, Line.find(';') + 1);        // Erase time stamp
	while(!Line.empty()) {
		if(Line.find('|') != string::npos) ApkNum++;        // Count APK number
		Line.erase(0, Line.find('|') + 1);        // Erase counted APK
	}
	return ApkNum;
}

int calculateApkNum(string &ReadName, fstream &ReadFile) {
	string LineLast;
	string LineCurrent;

	openFile(ReadName, ReadFile);
	while(getline(ReadFile, LineCurrent)) LineLast.assign(LineCurrent);
	ReadFile.close();
	return calNum(LineLast);
}

void recordApks(string ApkNames[50], string &Line) {
	int Index = 0;
	Line.erase(0, Line.find(';') + 1);        // Erase time stamp
	while(!Line.empty()) {
		ApkNames[Index].assign(Line.substr(0, Line.find(',')));
		Index++;
		Line.erase(0, Line.find('|') + 1);        // Erase recorded APK
	}
}

void recordApkNames(string ApkNames[50], string &ReadName, fstream &ReadFile) {
	string LineLast;
	string LineCurrent;

	openFile(ReadName, ReadFile);
	while(getline(ReadFile, LineCurrent)) LineLast.assign(LineCurrent);
	recordApks(ApkNames, LineLast);
	ReadFile.close();
}

void writeHtmlHeader(fstream &WriteToHere)
{
	WriteToHere << "<!doctype html>\n"
				<< "<html>\n<head>\n"
				<< "<meta http-equiv=Content-Type content=\"text/html; charset=utf8\">"
				<< "<style type=\"text/css\">\n"
				<< "body{font-family:Arial;background-color:#D2DCDD;}\n"
				<< "h2{background-color:#5A675D;color:white;}\n"
				<< "table{border-style:double;background-color:white;}\n"
				<< "td{border-width:1px;border-style:solid;}\n"
				<< "div{font-size:16px;}\n"
				<< "#tilte{font-weight:bolder;background-color:#DED9D5;}\n"
				<< "#battery{text-align:center;}\n"
				<< "</style>\n"
				<< "</head>\n"
				<< "<body>\n"
				<< "<table>\n";
}

void writeTableHeader(string ApkNames[50], int ApkNum, fstream &WriteFile) {
	WriteFile << "<tr>\n"
			  << "<td>Time</td>\n";
	for(int Index = 0 ; Index < ApkNum ; Index++) WriteFile << "<td>" << ApkNames[Index] << "</td>\n";
	WriteFile << "</tr>\n";
}

void writeTimeStamp(string &Line, fstream &WriteFile) {
	string Time;
	Time.assign(Line.substr(8, 4));
	Time.insert(2, ":");
	WriteFile << "<td>" << Time << "</td>\n";
	Line.erase(0, Line.find(';') + 1);
}

void writeRowData(string &Line, int ApkNum, fstream &WriteFile) {
	writeTimeStamp(Line, WriteFile);
	for(int Index = 0 ; Index < ApkNum ; Index++) {
		if(!Line.empty()) {
			WriteFile << "<td>" << Line.substr(Line.find(',') + 1, Line.find('|') - Line.find(',') - 1) << "</td>\n";
			Line.erase(0, Line.find('|') + 1);
		}
		else WriteFile << "<td>0</td>\n";
	}
	WriteFile << "</tr>\n";
}

void writeTail(fstream &WriteFile) {
	WriteFile << "</table>\n"
			  << "</html>\n";
}

int main(int argc, char *argv[]) {
	string ReadName = argv[1];
	string WriteName;
	string Line;
	WriteName.assign(ReadName);
	WriteName.replace(WriteName.find('l'), 3, "html");
	fstream ReadFile;
	fstream WriteFile;
	int ApkNum = 0;
	ApkNum = calculateApkNum(ReadName, ReadFile);
	//string ApkNames[ApkNum];
	string ApkNames[50];
	recordApkNames(ApkNames, ReadName, ReadFile);

	openFile(ReadName, ReadFile);
	openFile(WriteName, WriteFile);
	writeHtmlHeader(WriteFile);
	writeTableHeader(ApkNames, ApkNum, WriteFile);
	while(getline(ReadFile, Line)) writeRowData(Line, ApkNum, WriteFile);
	writeTail(WriteFile);
	ReadFile.close();
	WriteFile.close();
	return 0;
}
