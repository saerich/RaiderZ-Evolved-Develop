<?
	require_once("core.php");
	check_session();

	$sess_login = $_SESSION["Login"];
	$sess_level = $_SESSION["Level"];
	$sess_DB = $_SESSION["DB"];


	function print_menu_link($url, $menu_str, $level)
	{
		global $sess_level;
		if ($sess_level	<= $level)
		{
			echo ("<A href='$url' target='gapmain'>$menu_str</A><BR>\n");
		}

	}
?>
<HTML>
<BODY>
<table border=0 style="font-size:9pt">
<tr>
<td>
<a href='main.html' target="gapmain">HOME</A> <BR><BR>
<B>����� ����</B> <BR>

<?
	print_menu_link("ServerCurrentInfo.html", "���� ��Ȳ", 9);
	print_menu_link("AccountInfo.html", "���� ����", 5);
	print_menu_link("CharacterInfo.html", "ĳ���� ����", 5);
	print_menu_link("CharacterCareLog.html", "ĳ���� ������� ����", 5);
	print_menu_link("112.html", "�ҷ����� �Ű� ����", 5);

	print_menu_link("ShopBuyItem.html", "ĳ�������� ����", 3);
	print_menu_link("InsertItemToAccountItem.html", "�߾����࿡ ������ �߰�", 3);
?>



<BR>
<B>��������</B> <BR>
<?
	print_menu_link("connlog.html", "���ӷα� ����", 5);
	print_menu_link("ShopSellData.html", "ĳ�������� �������� ����", 5);
	print_menu_link("ShopBountyItemLog.html", "�Ϲݾ����� �������� ����", 5);
    print_menu_link("ShopPresentLog.html", "ĳ�������� ������� ����", 5);
	print_menu_link("ShopItemUseInfo.html", "ĳ�������� ����� ����", 5);

?>

</td>
</tr>

<tr>
<td>
<BR><b>�������� </B> <BR>
<?
	print_menu_link("ShopSingleItem.html", "�����ۼ� ����", 3);
	print_menu_link("ShopSetItem.html", "��Ʈ �����ۼ� ����", 3);
	print_menu_link("ShopNewItem.html", "�Ż�ǰ ����", 3);
	print_menu_link("ShopReceiptLog.html", "�����ۼ� �α�", 3);
?>
</TD>
</TR>

<tr>
<td>
<BR>
<b>���</B> <BR>
<?
	print_menu_link("Statistics_Total.html", "��� ����", 3);
	print_menu_link("Sale.html", "���� ��Ȳ", 3);

?>
</TD>
</TR>

<tr>
<td>
<BR>
<b>������</B> <BR>
<?
	print_menu_link("user_list.php", "����� ����", 1);
?>
</TD>
</TR>


<tr>
<td>
<BR><b>����</B><BR>
DB:
<?PHP
require_once('core.php');
echo ("$sess_DB");
#echo ("$g_db_name");
?>

<?
	if ($sess_level	<= 1)
	{
		echo ("(<A href='config.html' target='gapmain'><FONT color=#DDDDDD>modify</FONT></A>)\n");
	}


	echo ("<BR><A href='user_edit.php?userid=$sess_login&myself_edit=1' target='gapmain'>��������</A><BR>\n");

?>


</TD>
</TR>

<BR><BR>

<tr>
<td>
<BR><b>����</B><BR>

</TD>
</TR>


</table>
<BR>
</BODY>
</HTML>
