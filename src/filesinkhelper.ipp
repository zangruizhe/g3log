/** ==========================================================================
 * 2013 by KjellKod.cc. This is PUBLIC DOMAIN to use at your own risk and comes
 * with no warranties. This code is yours to share, use and modify with no
 * strings attached and no restrictions or obligations.
 *
 * For more information see g3log/LICENSE or refer refer to http://unlicense.org
 * ============================================================================*/

#pragma once


#include <memory>
#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>


namespace g3 {
   namespace internal {
      static const std::string file_name_time_formatted = "%Y%m%d-%H%M%S";

      // check for filename validity -  filename should not be part of PATH
      bool isValidFilename(const std::string &prefix_filename) {
         std::string illegal_characters("/,|<>:#$%{}()[]\'\"^!?+* ");
         size_t pos = prefix_filename.find_first_of(illegal_characters, 0);
         if (pos != std::string::npos) {
            std::cerr << "Illegal character [" << prefix_filename.at(pos) << "] in logname prefix: " << "[" << prefix_filename << "]" << std::endl;
            return false;
         } else if (prefix_filename.empty()) {
            std::cerr << "Empty filename prefix is not allowed" << std::endl;
            return false;
         }

         return true;
      }

      std::string prefixSanityFix(std::string prefix) {
         prefix.erase(std::remove_if(prefix.begin(), prefix.end(), ::isspace), prefix.end());
         prefix.erase(std::remove(prefix.begin(), prefix.end(), '/'), prefix.end());
         prefix.erase(std::remove(prefix.begin(), prefix.end(), '\\'), prefix.end());
         prefix.erase(std::remove(prefix.begin(), prefix.end(), '.'), prefix.end());
         prefix.erase(std::remove(prefix.begin(), prefix.end(), ':'), prefix.end());
         if (!isValidFilename(prefix)) {
            return
            {
            };
         }
         return prefix;
      }

      std::string pathSanityFix(std::string path, std::string file_name) {
         // Unify the delimeters,. maybe sketchy solution but it seems to work
         // on at least win7 + ubuntu. All bets are off for older windows
         std::replace(path.begin(), path.end(), '\\', '/');

         // clean up in case of multiples
         auto contains_end = [&](std::string & in) -> bool {
            size_t size = in.size();
            if (!size) return false;
            char end = in[size - 1];
            return (end == '/' || end == ' ');
         };

         while (contains_end(path)) {
            path.erase(path.size() - 1);
         }

         if (!path.empty()) {
            path.insert(path.end(), '/');
         }

         path.insert(path.size(), file_name);
         return path;
      }

      std::string header() {
         std::ostringstream ss_entry;
         //  Day Month Date Time Year: is written as "%a %b %d %H:%M:%S %Y" and formatted output as : Wed Sep 19 08:28:16 2012
         ss_entry << "\t\tg3log created log at: " << g3::localtime_formatted(g3::systemtime_now(), "%a %b %d %H:%M:%S %Y") << "\n";
         ss_entry << "\t\tLOG format: [YYYY/MM/DD hh:mm:ss uuu* LEVEL FILE:LINE] message";
         ss_entry << "\t\t(uuu*: microsecond counter since initialization of log worker)\n\n";
         return ss_entry.str();
      }

      std::string createLogFileName(const std::string &verified_prefix) {
         std::stringstream oss_name;
         oss_name << verified_prefix << ".g3log.";
         oss_name << g3::localtime_formatted(g3::systemtime_now(), file_name_time_formatted);
         oss_name << ".log";
         return oss_name.str();
      }

      bool openLogFile(const std::string &complete_file_with_path, std::ofstream &outstream) {
         std::ios_base::openmode mode = std::ios_base::out; // for clarity: it's really overkill since it's an ofstream
         mode |= std::ios_base::trunc;
         outstream.open(complete_file_with_path, mode);
         if (!outstream.is_open()) {
            std::ostringstream ss_error;
            ss_error << "FILE ERROR:  could not open log file:[" << complete_file_with_path << "]";
            ss_error << "\n\t\t std::ios_base state = " << outstream.rdstate();
            std::cerr << ss_error.str().c_str() << std::endl;
            outstream.close();
            return false;
         }
         return true;
      }

      std::unique_ptr<std::ofstream> createLogFile(const std::string &file_with_full_path) {
         std::unique_ptr<std::ofstream> out(new std::ofstream);
         std::ofstream &stream(*(out.get()));
         bool success_with_open_file = openLogFile(file_with_full_path, stream);
         if (false == success_with_open_file) {
            out.release();
         }
         return out;
      }

      std::string GetFilename (const std::string& str)
      {
        std::cout << "Splitting: " << str << '\n';
        std::size_t found = str.find_last_of("/\\");
        // std::cout << " path: " << str.substr(0,found) << '\n';
        // std::cout << " file: " << str.substr(found+1) << '\n';
        return str.substr(found+1);
      }

      std::string GetExecutionName() {
        char exe[1024];
        int ret;

        ret = readlink("/proc/self/exe",exe,sizeof(exe)-1);
        if(ret ==-1) {
          return "";
        }
        exe[ret] = 0;
        return GetFilename(std::string(exe)) + ".";
      }



      void CreatLinkToLogFile(const std::string& log_file_path, const std::string& execution__file_name) {
        std::string sys_cmd = "ln -sf ";
        // sys_cmd.append(log_file_path).append(" ").append(GetExecutionName()).append("LOG");
        sys_cmd.append(log_file_path).append(" ").append(execution__file_name).append(".LOG");
        system(sys_cmd.c_str());
      }
   }
}
