#!/usr/bin/env ruby
# -*- coding: utf-8 -*-

# Doxygen コメントに従って対象ファイルをコピーするときのコメント出力を調整する

$KCODE = "UTF-8"


if ARGV.empty?
  print "usage: \n" +
    "    " + __FILE__ + " [options] <files> <directory> \n" +
    "\n" +
    "[options]\n" +
    " -e      output english comment.\n" +
    " -j      output Japanese comment.\n" +
    "\n"
  exit(1)
end


# 最後の引数がディレクトリでなければ、エラーメッセージを返す
output_directory = ARGV.pop
output_mode = ARGV.shift
if not FileTest::directory?(output_directory)
  print "No such directory: " + output_directory + "\n"
  exit(1)
end
target_files = ARGV


def split_single_line(line, mode, output_mode)

  # 現在のモードに従ったコメントのみを出力する
  if line =~ /\\\~japanese .+ ([\*\\])/ and output_mode == "-e"
    line = $` + $1 + $'
  end
  if line =~ /\\\~english .+ ([\*\\])/ and output_mode == "-j"
    line = $` + $1 + $'
  end

  return line
end


def remove_matched_word(line)

  if line.strip == ""
    line = ""
  end
  line
end


# Doxygen コメントに従って対象ファイルをコピーするときのコメント出力を調整する
def split_comment(file_name, output_mode)
  lines = ""
  mode = "both"
  is_comment = false

  File.open(file_name) { |fd|
    fd.each { |line|

      if is_comment
        # コメント中
        case line
        when /\\\~japanese/
          mode = "japanese"
          line = $` + $'
          if line.strip == ""
            line = ""
          end
        when /\\\~english/
          mode = "japanese"
          line = $` + $'
          if line.strip == ""
            line = ""
          end
        when /\\\~/
          mode = "both"
          line = remove_matched_word($` + $')
        end
        if line =~ /\*\//
          is_comment = false
          lines += line
        else
          # モードに従ってコメントを出力する
          if mode == "japanese" and output_mode == "-j"
            lines += line
          elsif mode == "english" and output_mode == "-e"
            lines += line
          elsif mode == "both"
            lines += line
          end
        end
      else
        # ソースコード中
        if line =~ /\/\*/
          if line =~ /\*\//
            is_comment = false
            line = split_single_line(line, mode, output_mode)
          else
            is_comment = true
            mode = "both"
          end
        else
          # // コメントの場合の処理
          case line
          when /~japanese/
            line = split_single_line(line, mode, output_mode)
          when /~english/
            line = split_single_line(line, mode, output_mode)
          end
        end
        lines += line
      end
    }
  }

  lines
end


# 対象のファイルを処理しつつ、指定されたディレクトリにコピーする
target_files.each { |file_name|
  converted_lines = split_comment(file_name, output_mode)
  converted_file_name = output_directory + "/" + File.basename(file_name)

  File.open(converted_file_name, "w") { |fd|
    fd.print converted_lines
  }
}
