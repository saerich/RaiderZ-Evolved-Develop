<?php
	$no = $_GET['no'];
	if ($no == '') $no = $_POST['no'];
	if (!$no || $no < 0) $no=0; //$no ���� �ȳѾ� ���ų� �߸���(����)���� �Ѿ���� ��� 0���� ó��

	$page_size=15;
	$page_list_size = 15;

	$table_width = 800;
	
	// db
	require_once("mdb2.php");
	require_once("feedback_common.php");
	
?>
<HTML>
<HEAD>
<TITLE>�ǵ�� �Խ���</TITLE>
<LINK REL='StyleSheet' HREF='style.css'>
<STYLE>
BODY { font-size: 9pt;  font-family:����; color:#333333}
TD { font-size: 9pt; font-family:����; color:#333333}
tx2 { font-size: 9pt;  font-family:����;}

/*COLOR*/
.gray0 {color:#292929;}
.gray1 {color:#333333;}
.gray2 {color:#565656;}
.gray2a {color:#6A6A6A;}
.gray3 {color:#CECECE;}
.gray4 {color:#F4F4F4;}
.gray5 {color:#F1F1F1;}
.blue {	color: #0052BC; }
.blue1 { color: #0C6CA6; }
.qblue { color: #0073C0; }
.deepblue {color:#004A81;}
.cblue {color:#323491;}
.evblue {color:#0055A5;}
.black {color:#000000;}
.orange {color:#F36523;}
.white {color:#FFFFFF;}
.pink {color: #F51870;}
.yellow {color: #FFFD7E;}
.yellow2 {color: #BC9C6F;}
.red {color: #EA0001;}
.bro1 {color: #733906;}
.bro2 {color: #603B1A;}
.y1 {color: #F8E657;}

/* �⺻��ũ */
A:link   	{ color: #333333; TEXT-DECORATION: none}
A:visited	{ color: #333333; TEXT-DECORATION: none}
A:active 	{ color: #333333; TEXT-DECORATION: none}
A:hover  	{ color: #333333; TEXT-DECORATION: underline}

</STYLE>
<SCRIPT>
var __select_mode = 0;

function select_message()
{
	var value;
	var o = document.all['allsel'];
	if ( __select_mode == 0 )
	{
		value = true;
		__select_mode = 1;
		o.innerHTML = "��ü����";
	}
	else
	{
		value = false;
		__select_mode = 0;
		o.innerHTML = "��ü����";
	}
	o = document.f.chkbox;
	if ( o != null )
	{
		var olen = eval(o.length);
		if ( olen != null )
		{
			for ( var i = 0 ; i < o.length ; i++ )
			{
				o[i].checked = value;
			}
		}
		else
		{
			o.checked = value;
		}
	}
}

function get_sequence_string()
{
	var s = "";
	var o = document.f.chkbox;
	if ( o != null )
	{
		var lenobj = eval(o.length);
		if(lenobj != null)
		{
			for ( var i = 0 ; i < o.length ; i++ )
			{
				if ( o[i].checked )
					s += (( s.length > 0 ? "," : "" ) + o[i].value );
			}
		}
		else {
			if ( o.checked )
				s += (( s.length > 0 ? "," : "" ) + o.value );
		}
	}
	return s;
}


function delete_message()
{
	var s = get_sequence_string();
	if ( s == "" )
	{
		alert( "���õ� �Խù��� �����ϴ�." );
		return;
	}
	if ( confirm( "������ �Խù��� ������ �����ұ��?" ) )
	{
<?
	echo("			location = 'feedback.php?mode=delete_process&no=" . $no . "&del=' + s ;");
?>
	}
}



</SCRIPT>
</HEAD>
<BODY>
<?php
// �������� ///////////////////////////////////////////////////////////////////////
$mode = $_GET['mode'];
if ($mode == '') $mode = $_POST['mode'];

$REMOTE_DIR = sprintf("feedback/");
$FTP_USER = "dev";
$FTP_PASS = "dev";
$FTP_SERVER = "icecream";


echo("<CENTER><H1>�ǵ�� �Խ���</H1></CENTER>\n");

// �Լ� ///////////////////////////////////////////////////////////////////////////

function get_state_str($state)
{
	if (($state == 0) || ($state == '')) return "�ű�";
	else if ($state == 1) return "ó����";
	else if ($state == 2) return "�Ű�ƴ�";

	return "";
}


function get_filedate_str($filename)
{
	list($day, $time, $name, $file_temp) = split("_", $filename);
	$date_str = substr($day, 0, 2) . "��" . substr($day, 2, 2) . "�� " . substr($time, 0, 2) . "��" . substr($time, 2, 2) . "��";

	return $date_str;
}

function get_reporter_from_filename($filename)
{
	list($day, $time, $name, $file_temp) = split("_", $filename);
	return $name;
}

function print_menubar()
{
	global $page_size;
	global $no;
	$prev = $no-$page_size;
	if ($prev <= 0) $prev=0;
	$next = $no+$page_size;

	echo("<TABLE border=0 cellspacing=0 cellpadding=0 width='100%'>
<tr  bgcolor=#89BDE7><td width=1  colspan=4></td></tr>
<TR  bgcolor=#BDDBF7>
<TD height=28 valign=middle>
<a href='feedback.php?no=$prev'><img src='./img/btn_ma_b.gif' border=0></A><a href='feedback.php?no=$next'><img src='./img/btn_ma_n.gif' border=0></A>
</TD>
<TD height=28 width=200></TD>
<td align=right style='padding:3 7 0 0' class='black' width=150>���õ� �Խù��� <a href='javascript:delete_message()'><img src='./img/btn_ma_d.gif' border=0></a></td>
<td align=right style='padding:3 7 0 0' class='black' width=10> &nbsp;</td>
</TR>
<tr bgcolor=#A8CDEF><td height=2 colspan=4></td></tr>
<tr bgcolor=#74A9D1><td height=1 colspan=4></td></tr>
<tr bgcolor=#CDDFEB><td height=1 colspan=4></td></tr>

</TABLE>
");


}


function print_navibar($_no, $_total_row, $_page_size, $_page_list_size)
{
	if ($_total_row <= 0) $_total_row = 0; // �ѰԽù��� ���� ���ų� �Ұ�� �⺻������ ����
	$_total_page = floor(($_total_row - 1) / $_page_size); // �ѰԽù��� ������ ������� ������ ������ �Ѵ�.

	$_current_page = floor($_no/$_page_size);		// ���� ������ ���

	$_start_page = (int)($_current_page / $_page_list_size) * $_page_list_size;	// ������ ����Ʈ
	$_end_page = $_start_page + $_page_list_size - 1;
	if ($_total_page < $_end_page) $_end_page = $_total_page;

	if ($_start_page >= $_page_list_size) {

	    # ���� ������ ����Ʈ���� ù ��° ���������� �� ������ �����ϸ� �ȴ�.
	    # $_page_size �� �����ִ� ������ �۹�ȣ�� ǥ���ϱ� ���ؼ��̴�.
	    $_prev_list = ($_start_page - 1)*$_page_size;
	    echo  "<a href=\"$PHP_SELF?no=$_prev_list\">����15����</a> \n";
	}


	# ������ ����Ʈ�� ���
	for ($i=$_start_page;$i <= $_end_page;$i++) {

		$page=$_page_size*$i; // ���������� no ������ ��ȯ.
		$_page_num = $i+1; // ���� ������ ���� 0���� �����ϹǷ� ǥ���Ҷ��� 1�� �����ش�. ������ 0 -> 1

	    if ($_no!=$page){ //���� �������� �ƴ� ��츸 ��ũ�� ǥ��
	        echo "<a href=\"$PHP_SELF?no=$page\">";
	    }
		if ($_no==$page){ echo ("<FONT color=red>"); }

	    echo " $_page_num "; //�������� ǥ��

	    if ($_no==$page){ echo ("</FONT>"); }

	    if ($no!=$page){
	        echo "</a> ";
	    }
	}

	# ���� ������ ����Ʈ�� �ʿ��Ҷ��� �� �������� ������ ����Ʈ���� Ŭ ���̴�.
	# ����Ʈ�� �� �Ѹ��� �� �ѷ��� �������� �������� ���� ��ư�� �ʿ��� ���̴�.
	if($_total_page > $_end_page)
	{
	    # ���� ������ ����Ʈ�� ������ ����Ʈ ���������� �� ������ �����ϸ� �ȴ�.
	    $_next_list = ($_end_page + 1)* $_page_size;
	    echo " <a href=$PHP_SELF?no=$_next_list>������15��</a><p>";
	}


}


function Replace_Comment($cmt)
{
  $out = "";
  $lines = explode("\n", $cmt);
  $cnt = count($lines);
  for ($i = 0; $i < $cnt; $i++)
  {
    if (strstr($lines[$i], ":") == FALSE)
    {
      $out = $out . "<FONT color=#AAAAAA>" . $lines[$i] . "</FONT>" . "\n";
    }
    else
    {
      $out = $out . $lines[$i] . "\n";
    }
  
  }
  return $out;
}

// ���� ////////////////////////////////////////////////////////////////////////////

if ($mode == "")
{
	$db = new FeedbackDB();
	$db->Open();
	
	$query = "SELECT fid, name, field, pos, dir, comment, RegDate FROM (SELECT ROW_NUMBER() OVER (ORDER BY fid DESC) AS Row, fid, name, field, pos, dir, comment, RegDate FROM Feedback) AS Feedback " .
	         "WHERE Row > $no AND Row <= ($no+$page_size) " .
			 "ORDER BY fid DESC";

	$db->Query($query);
	
	echo ("<FORM action='feedback.php' method=post name=f>\n");
	echo ("<CENTER>
	<TABLE border=0 cellspacing=0 cellpadding=0 width=$table_width>
<TR bgcolor=#FFFFFF>
<TD align=right><a href='feedback.php?mode=fetch&return=1'>��������</a></TD>
</TR>
<TR>
<TD>
");

	print_menubar();
echo("
</TD>
</TR>
<TR bgcolor='#E5F1FA'>
<TD>
<!-- menu -->
<TABLE  width='100%' border=0 cellspacing=0 cellpadding=0 bgcolor=#E5F1FA height=26>
<TR style='padding:3 0 0 0'>
<td width=8 nowrap></td>
<TD align=center width=60 class='deepblue' nowrap><a href='javascript:select_message();'><span id=allsel><u>��ü����</u></span></a></TD>
<TD align=left width='100' nowrap>&nbsp;</TD>
<TD align=center width='90%' nowrap>&nbsp;</TD>
</TR>
</TABLE>
<!-- menu end -->

<TR><TD>
<!-- list -->
<TABLE  width='100%' border=0 cellspacing=0 cellpadding=0 bgcolor=#FFFFFF>
");

	while ($row = $db->Fetch())
	{
  		$fid = $row['fid'];
	  	$name = stripslashes($row['name']);
		$field = $row['field'];
		$pos = $row['pos'];
		$dir = $row['dir'];
	  	$comment = nl2br(stripslashes($row['comment']));
		$regdate = $row['regdate'];


		$title_str = "<A href='feedback.php?mode=view&no=" . $no . "&rid=" . $fid . "'>" . $title . "</A>";

		echo("<TR style='padding:3 0 0 0' onMouseOver=\"this.style.backgroundColor='#F4F9FD';return true;\" onMouseOut=\"this.style.backgroundColor='#ffffff';return true;\">
<td width=8 nowrap></td>
<TD align=left valign=top width=80 nowrap>&nbsp;&nbsp;<input type=checkbox NAME='chkbox' VALUE='$fid'>&nbsp;$fid</TD>
<TD align=left valign=top width='100' nowrap><B>$name</B></TD>
<TD align=left width='90%' nowrap>$comment
<br/>
time( $regdate ) , field( $field ) , pos ( $pos )
<br/>
</TD>
</TR>
<tr><td colspan=3 height=1 bgcolor=#ECECEC></td></tr>
");

	} // while


	echo ("</TD></TR></TABLE>\n");

	echo ("<TR><TD>\n");

	print_menubar();

	echo ("</TD></TR><TR><TD>\n");

	$db->query("SELECT COUNT(*) AS total_cnt FROM Feedback");
	$row = $db->Fetch();
	$total_row = $row['total_cnt'];

	print_navibar($no, $total_row, $page_size, $page_list_size);


	echo ("</TD></TR>\n");
	echo ("</TABLE></CENTER>\n");

	$db->close();


}	// if ($mode == "")
else if ($mode == "fetch")
{
	$getvalue_return = $_GET['return'];
	
	set_time_limit (500);

	$ftp_conn = ftp_connect($FTP_SERVER, 21);
	if (!$ftp_conn) { die("Cannot connect to $FTP_SERVER"); }

	ftp_set_option($ftp_conn, FTP_TIMEOUT_SEC, 300);

	$LOGIN = ftp_login($ftp_conn, $FTP_USER, $FTP_PASS);
	if (!$LOGIN) { die ("Cannot login to $FTP_SERVER"); }
	if (!ftp_chdir($ftp_conn, $REMOTE_DIR)) { die("Cannot change directory"); }

	$f_lists = ftp_nlist($ftp_conn, ".");


	if (is_array($f_lists) && (count($f_lists) > 0))
	{
		// db open
		$db = new FeedbackDB();
		$db->Open();

		sort($f_lists);

		$cnt = 0;
		foreach($f_lists as $filename)
		{
			$cnt++;
			$filesize = ftp_size($ftp_conn, $filename);

			$name = "";
			$comment = "";
			$field = "";
			$pos = "";
			$dir = "";

			$localfile = "./feedback_temp.txt";

			$title = "";

			if (ftp_get($ftp_conn, $localfile, $filename, FTP_ASCII))
			{
				$fp = fopen($localfile, "rt");

				$line = 0;
				while (!feof ($fp))
				{
					$buffer = fgets($fp, 1024);

					if ($line == 0)
					{
						$name = $buffer;
					}
					else if ($line == 1)
					{
						$field = $buffer;
					}
					else if ($line == 2)
					{
						$pos = $buffer;
					}
					else if ($line == 3)
					{
						$dir = $buffer;
					}
					else
					{
						$comment .= $buffer;
					}
					$line++;
				}
				fclose($fp);
			}

			$comment = addslashes($comment);

			$query = "INSERT INTO Feedback(Name, Field, pos, dir, Comment, RegDate) VALUES (" .
			         "'$name', $field, '$pos', '$dir', '$comment', GETDATE())";
	
			//echo $query . "<BR>\n";

			$db->query($query);

			// db�� ����� ������ ����
			ftp_delete($ftp_conn, $filename);
		}
		
		$db->Close();
	}

	ftp_close($ftp_conn);
	ftp_quit($ftp_conn);

	if ($cnt == '') $cnt = 0;
	
	if ($getvalue_return == 1)
	{
		echo ("<meta http-equiv='Refresh' content='1; URL=feedback.php?no=$no'>");
	}
	echo ("<center><font size=2>". $cnt . "���� ���ο� �ǵ���� ��ϵǾ����ϴ�.</font></center>\n");
}
else if ($mode == "delete")
{
	$r_rid = $_POST['rid'];

	echo ("<CENTER>
	������ �����Ͻðڽ��ϱ�?
	<FORM action='feedback.php' method=post>
		<INPUT type=hidden name='no' value='$no'>
		<INPUT type=hidden name=rid value='$r_rid'>
		<INPUT type=hidden name=mode value='delete_process'><INPUT type=submit value='�����ϱ�'></TD>
	</FORM>
	</CENTER>\n");
}
else if ($mode == "delete_process")
{
	$r_del = $_GET['del'];

	$del_count = 0;

	if ($r_del != '')
	{
		$fids = explode(",", $r_del);
		$del_count = count($fids);
	}
	else
	{
		$r_rid = $_POST['rid'];
		if ($r_rid == "")
		{
			exit();
		}

		$fids[0] = $r_rid;
		$del_count = 1;
	}

	$db = new FeedbackDB();
	$db->Open();

	for ($i = 0; $i < $del_count; $i++)
	{
		$query = "DELETE FROM Feedback WHERE fid=" . $fids[$i];
		$result = $db->Query($query);
	}

	if (!$result)
	{
		$db->Close();
		exit();
	}

	$db->Close();



	echo ("<meta http-equiv='Refresh' content='0; URL=feedback.php?no=$no'>");
	echo ("<center><font size=2>�����Ǿ����ϴ�.</font></center>\n");
}
#################################
else if ($mode == "view")
{
	$r_rid = $_GET['rid'];
	if ($r_rid=='') $r_rid=0;

	$db = new FeedbackDB();
	$db->Open();
	
	$query = "SELECT fid, reporter, title, comment, regdate FROM Feedback WHERE fid = " . $r_rid;
	
	$db->Query($query);

	$row = $db->Fetch();


	$reporter = stripslashes($row['reporter']);
	$title = stripslashes($row['title']);
	$filename = "";
	
  $comment = Replace_Comment($row['comment']);	
	$comment = nl2br(stripslashes($comment));
	$admin_comment = stripslashes($row['admin_comment']);
	$state = $row->state;
	$regdate = $row['regdate'];


	$filedate = get_filedate_str($filename);


	$db->Close();

	echo ("<CENTER>
<BR>
<TABLE border=0 cellpadding=1 cellspacing=1>
<TR><TD bgcolor=#EEEEE align=center width=120 height=30><B>������</B></TD><TD bgcolor=#F0F0F0 width=480>$reporter</TD></TR>
<TR><TD bgcolor=#EEEEE align=center height=30><B>����</B></TD><TD bgcolor=#F0F0F0>$title</TD></TR>
<TR><TD bgcolor=#EEEEE align=center height=30><B>������</B></TD><TD bgcolor=#F0F0F0>$regdate</TD></TR>
<TR><TD bgcolor=#EEEEE align=center height=30><B>���ϳ�¥</B></TD><TD bgcolor=#F0F0F0>$filedate</TD></TR>
<TR><TD bgcolor=#EEEEE align=center height=300><B>����</B></TD><TD bgcolor=#F0F0F0 valign=top>$comment
");

	echo("</TD></TR></TABLE>\n");
	echo("<BR>");
	echo("<TABLE border=0 cellpadding=0 cellspacing=1 width=600 height=100 bgcolor=#333333>
<TR><TD>
<TABLE border=0 cellpadding=3 cellspacing=1 bgcolor=white width=100% height=100%>
<TR>
<TD colspan=2><B>������ ó��</B></TD>
</TR>
<TR>
	<FORM method='post' action='./feedback.php'>
	<INPUT type=hidden name='no' value='$no'>
	<INPUT type=hidden name='mode' value='edit'>
	<INPUT type=hidden name='rid' value='$r_rid'>
<TD width=120 bgcolor=#EEEEE align=center><B>����</B></TD>
<TD bgcolor=#eeeeee>
<SELECT name='state'>
");

	for ($i = 0; $i <=2; $i++)
	{
		$selected_str = "";
		if ($state == $i) $selected_str = " selected";

		echo ("<OPTION value=$i $selected_str >" . get_state_str($i) . "</OPTION>\n");
	}

echo("
</SELECT></TD>
</TR>
<TR>
<TD bgcolor=#EEEEE align=center><B>������ �ǰ�</B></TD>
<TD bgcolor=#eeeeee>
<TEXTAREA name='admin_comment' cols=50 rows=6 wrap=VIRTUAL style='border-style:solid;border-width:1px;border-color:#dadada; vertical-align:top;bgcolor:#F8F4F3'>$admin_comment</TEXTAREA></TD>
</TR>
<TR>
<TD colspan=2 align=right>
	<TABLE border=0 cellpadding=3>
	<TR>
		<TD valign=top width=90><INPUT type=submit value='�����ϱ�'></FORM></TD>
		<TD valign=top width=90><FORM method='post' action='./feedback.php'>
			<INPUT type=hidden name='no' value='$no'>
			<INPUT type=hidden name='mode' value='delete'>
			<INPUT type=hidden name='rid' value='$r_rid'>
			<INPUT type=submit value='�����ϱ�'>
			</FORM>
		</TD>
		<TD valign=top width=90><FORM method='post' action='./feedback.php'>
			<INPUT type=hidden name='no' value='$no'>
			<INPUT type=hidden name='mode' value=''>
			<INPUT type=submit value='��Ϻ���'>
			</FORM>
		</TD>
	</TR>

	</TABLE>
</TD>
</TR>
</TABLE>
</TD></TR>
</TABLE>
<BR>


");



	echo("</CENTER>\n");
}

?>


<? include ("sign.inc"); ?>
</BODY>
</HTML>
