<html>
 <head>
  <title>�ҿ���Ʈ ����͸�</title>
 </head>
 <body>
 
 <H2><center>�ҿ���Ʈ ����͸�</center></H2>
 <br/>
 
 <?php 
 include "main.php";
 ?>
 
 <div id='recent'>
 <center>
 
 <H3><center>�ֱ� ��Ȳ</center></H3>
 
 <table border=1>
 <tr>
 	<td width=150>��ǻ��</td><td width=200><?php echo $replay_data->GetLastComputer(); ?></td>
</tr>
<tr>
	<td>FPS</td><td><?php echo $replay_data->GetLastFPS(); ?></td>
</tr>
<tr>
	<td>Loading Time</td><td><?php echo $replay_data->GetLastLoadingTime(); ?> ��</td>
</tr>
 <tr>
	<td width=150>�ֱ� ���� �ð�</td><td width=200><?php echo $replay_data->GetLastRegTimeStr(); ?></td>
</tr>

</table>
</center>
 </div>
 
 <br/><br/>
 <div id='graph'>
 <center>
 <H3>FPS �׷���</H3>
 
 <br/>
 <?php $chart->PrintFPSChartHtml($replay_data); ?>
 
 <br/><br/>
 <H3>Loading Time �׷���</H3>
 
 
 <br/>
 <?php $chart->PrintLoadingTimeChartHtml($replay_data); ?>
 
 </center>
 </div>
 
 
 <br/><br/>
 
 <div id='screenshot'>
 <center>
 <H3>�ֱ� ��ũ����</H3>
 
 <?php PrintScreenShot(); ?>
 </center>
 </div>
 
 <br/><br/><br/>
 
 <div id='history'>
 <center>
 <H3>History</H3>
 
  <?php PrintHistory($replay_data); ?>
 
 </center>
 </div>
 
 <br/><br/>
 
 </body>
</html>