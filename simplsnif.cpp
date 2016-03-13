#include <iostream>
#include <string.h>
#include <pcap.h>
#include <stdlib.h>
#include <arpa/inet.h>


#define MAXBYTESTOCAPTURE 2048
#define ETHERNET_HEADERS_SIZE 14
#define IP_ADDR_OFFSET 12

using namespace std;

string http_header= "";
string all_content = "";
string content_type = "";
int content_length;
bool header_ended = false;
//bool is_request = false;
//bool is_response = false;
bool http_process = false;

string get_content_type(string http);
int get_content_length(string http);
string get_login_and_pass(string content);


void processPacket(u_char *arg, const struct pcap_pkthdr* pkthdr, const u_char *packet){
    char ip_src[4];
    char ip_dst[4];
    int ip_headers_length = (packet[ETHERNET_HEADERS_SIZE] & 0x0F)*4;
    int tcp_headers_length = ((packet[ETHERNET_HEADERS_SIZE+ip_headers_length+12] & 0xF0)>>4)*4;
    int payload_offset;
    
    for(int i = ETHERNET_HEADERS_SIZE+IP_ADDR_OFFSET, j = 0; i<ETHERNET_HEADERS_SIZE+IP_ADDR_OFFSET+4; i++, j++){
        ip_src[j] = packet[i];
        ip_dst[j] = packet[i+4];
    }
    
    payload_offset = ETHERNET_HEADERS_SIZE+ip_headers_length+tcp_headers_length;
    
    for(int i = payload_offset; i<pkthdr->len; i++){

        if(!header_ended) http_header+=(char)packet[i];
        all_content+=(char)packet[i];
    }
    
    if(!header_ended){
        int l = (int)http_header.length();
        int cl = (int)all_content.length();
        for(int i = 0; i < l; i++){
            if( http_header[i]=='\r' && http_header[i+1]=='\n' && http_header[i+2]=='\r' && http_header[i+3]=='\n'){
                http_header = http_header.substr(0, i);
                header_ended = true;
                if(i+4<cl-1) all_content = all_content.substr(i+4, cl);
                break;
            }
        }
    }
    
    if(header_ended){
        if(!http_process){
            content_type = get_content_type(http_header);
            content_length = get_content_length(http_header);
            http_process = true;
        }
        if(all_content.length()>=content_length){
            cout << "New packet \n";
            cout << "IP DST: " << inet_ntoa(*(struct in_addr*)ip_dst) << "\n";
            cout << "IP SRC: " << inet_ntoa(*(struct in_addr*)ip_src) << "\n";
            cout<<"Content-Type:" <<content_type<<"\n";
            cout<<"Content-Length:"<<content_length<<"\n";
            if(content_type.find("urlencoded")!=string::npos){
                int start = 0;
                int len = 1;
                for(int i = 0; i<(int)all_content.length(); i++, len++){
                    if(all_content[i]=='&'){
                        cout << all_content.substr(start, len-1) << '\n';
                        start = i+1;
                        len = 0;
                    }
                }
            }
            //string lp = get_login_and_pass(all_content);
            //cout << lp <<'\n';
            //cout<<http_header<<"\n";
            //cout<<all_content<<"\n\n";
            cout<< "End of packet \n";
            http_process = false;
            header_ended = false;
            http_header = "";
            all_content = "";
        }
    }
    
}

string get_content_type(string http){
    int pos_start, length = 0;
    int l = (int)http.length();
    if(http.find("Content-Type")==string::npos){
        return "Content-Type: Undefined";
    }
    else{
        pos_start = (int)http.find("Content-Type");
    }
    for(int i = pos_start+14; i<l; i++, length++){
        if(http[i]==13){
            break;
        }
    }
    return http.substr(pos_start+14, length);
}
int get_content_length(string http){
    int pos_start, length = 0;
    int l = (int)http.length();
    if(http.find("Content-Length")==string::npos){
        return 0;
    }
    else{
        pos_start = (int)http.find("Content-Length");
    }
    for(int i = pos_start+16; i<l; i++, length++){
        if(http[i]==13){
            break;
        }
    }
    string ls = http.substr(pos_start+16, length);
    return stoi(ls);
}

string get_login_and_pass(string content){
    int log_pos, pass_pos,length = 0;;
    int l = (int)content.length();
    if(content.find("UserName")==string::npos || content.find("PassWord")==string::npos){
        return "Login And Pass not found";
    }
    else{
        log_pos = (int)content.find("UserName");
        pass_pos= (int)content.find("PassWord");
    }
    for(int i = log_pos; i<l; i++, length++){
        if(content[i]=='&') break;
    }
    string login = content.substr(log_pos, length);
    length = 0;
    for(int i = pass_pos; i<l; i++, length++){
        if(content[i]=='&') break;
    }
    string pass = content.substr(pass_pos, length);
    return login+" "+pass;
}

int main(int argc, const char * argv[]) {
    //init variables
    string all;
    pcap_t *descriptor = NULL;
    char errorBuffer[PCAP_ERRBUF_SIZE], *device = NULL;
    string settings = "tcp port 80 and (((ip[2:2] - ((ip[0]&0xf)<<2)) - ((tcp[12]&0xf0)>>2)) != 0)";
    //string settings = "http";
    bpf_u_int32 netp, maskp;
    memset(errorBuffer, 0, PCAP_ERRBUF_SIZE);
    struct bpf_program fp;
    
    //setting deveice to use
    device = pcap_lookupdev(errorBuffer);
    
    //getting net parameters
    pcap_lookupnet(device, &netp, &maskp, errorBuffer);
    cout << device << "\n";
    
    //setting descriptor
    descriptor = pcap_open_live(device, MAXBYTESTOCAPTURE, 1, 512, errorBuffer);
    
    //setting filters
    pcap_compile(descriptor, &fp, settings.c_str(), 0, netp);
    pcap_setfilter(descriptor, &fp);
    
    //run sniff loop
    pcap_loop(descriptor, -1, processPacket, (u_char*)&all);
    
    
    return 0;
}
