<?php
	$login_id = stripslashes($_COOKIE["soap_id"]);
?>
<HTML>
<HEAD>
<TITLE>���̿�Ʈ �ҿ���Ʈ ��� ���� ����Ʈ(SOAP)</TITLE>
<LINK REL='StyleSheet' HREF='style.css'>
</HEAD>
<BODY BGCOLOR="#F4EBE6">
<br>
<center>
<IMG src='./img/logo_title.jpg' border=0>
<BR><br><br>
<table width=40%  cellpadding=1 cellspacing=1>
<tr>
	<td bgcolor=#AAAAAA>
		<table width=100% cellpadding=1 cellspacing=1 border=0>
		<tr>
			<td bgcolor=e1e1e1>
				<BR>
				<center>
				<b>�ҿ���Ʈ ��� �α���</b>
				<form method=post action="login_action.php" name='login'>
				<br>
				<table>
				<tr>
					<td>�� �� ��</td>
<?php
	echo ("<td><input type='text' size=20 name='username' STYLE='height=19px;border: 1px solid  #666666;background-color: #ffffff;' value=$login_id> &nbsp;</td>\n");
?>
				</tr>
				<TR>
					<td>��й�ȣ</td>
					<td><input type="password" size=20 name="passwd" STYLE="height=19px;border: 1px solid  #666666;background-color: #ffffff;"> &nbsp;</td>
				</TR>
				<TR>
					<td>&nbsp; </td>
					<td><input type=image src='img/btn_log.gif'></td>
				</TR>
				</table>
				</form>
			</td>
		</tr>
		</table>
	</td>
</tr>
</table>
</body>
<script>
<?php
	if ($login_id == '') echo ("login.username.focus();\n");
	else echo ("login.passwd.focus();\n");
?>
</script>
</html>
