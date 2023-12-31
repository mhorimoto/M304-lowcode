<!DOCTYPE html>
<html lang="ja">
  <head>
    <meta charset="utf-8">
    <title>UECS for M304</title>
    <link href="css/main.css" rel="stylesheet" type="text/css">
  </head>
  <body id="tools">
    <div id="t-header">
      <a name="pagetop"></a>
      <h1 id="tools">M304 builder</h1>
    </div>
    <div id="wrapper">
      <h2>NODE INFO</h2>
      <form action="index.php" method="POST">
	<div class="d02 ws-padding">
	  <table class="t04">
	    <tr>
	      <td>
		{include file='node_part.tpl'}
	      </td>
	      <td>
		{include file='ip_part.tpl'}
	      </td>
	    </tr>
	  </table>
	</div>
	<h2>送信CCM情報設定</h2>
	<div class="d02 ws-padding">
	  <table class="t05">
	    <tr>
	      <th rowspan="2">No.</th>
	      <th width="250">値設定</th>
	      <th>桁数</th>
	      <th>単位</th>
	      <th>Lev</th>
	      <th>ROOM/REGION/ORDER/PRIORITY</th>
	    </tr>
	    <tr>
	      <th>CCM名称</th>
	      <th colspan="4"></th>
	    </tr>
	    <tr>
	      <td rowspan="2">0</td>
	      <td>[詳細設定](未サポート)</td>
	      <input type="HIDDEN" name="SENSOR[0]" value="condition">
	      <td><input type="HIDDEN" name="CAST[0]" value="0">0</td>
	      <td><input type="HIDDEN" name="UNIT[0]" value="">None</td>
	      <td>A-1S-0</td>
	      <input type="HIDDEN" name="SRLev[0]" value="A-1S-0">
	      <td>
		<input type="NUMBER" name="ROOM[0]" class="txt3" min="0" max="127" size="3" maxlength="3" value="1">/
		<input type="NUMBER" name="REGION[0]" class="txt3" min="0" max="127" size="3" maxlength="3" value="1">/
		<input type="NUMBER" name="ORD[0]" class="txt5" min="0" max="30000" size="5" maxlength="5" value="1">/29
		<input type="HIDDEN" name="PRIORITY[0]" value="29">
	      </td>
	    </tr>
	    <tr>
	      <td><input type="TEXT" name="CCMTYPE[0]" size="20" maxlength="20" value="cnd"></td>
	      <td colspan="4">設定内容<input type="HIDDEN" name="DEVICE[0]" value=""></td>
	    </tr>
	    {for $idx=1 to 7}
	    <tr>
	      <td rowspan="2">{$idx}</td>
	      <td>
		<select name="SENDVAL[{$idx}]">
		  <option value="ALLRLYSTAT">すべてのRelayの状態</option>
		  <option value="RLY1STAT">Relay1の状態</option>
		  <option value="RLY2STAT">Relay2の状態</option>
		  <option value="RLY3STAT">Relay3の状態</option>
		  <option value="RLY4STAT">Relay4の状態</option>
		  <option value="RLY5STAT">Relay5の状態</option>
		  <option value="RLY6STAT">Relay6の状態</option>
		  <option value="RLY7STAT">Relay7の状態</option>
		  <option value="RLY8STAT">Relay8の状態</option>
		</select>
	      </td>
	      <td><input type="NUMBER" name="CAST[{$idx}]" class="txt3" min="0" max="9" size="1" maxlength="1" value="0"></td>
	      <td><input type="TEXT" name="UNIT[{$idx}]" size="10" maxlength="10" placeholder="単位" value=""></td>
	      <td>
		<select name="SLEV[{$idx}]">
		  <option value="A1S0">A-1S-0</option>
		  <option value="A1S1">A-1S-1</option>
		  <option value="A10S0" selected>A-10S-0</option>
		  <option value="A10S1">A-10S-1</option>
		  <option value="A1M0">A-1M-0</option>
		  <option value="A1M1">A-1M-1</option>
		  <option value="B0">B-0</option>
		  <option value="B1">B-1</option>
		  <option value="S1S0">S-1S-0</option>
		  <option value="S1M0">S-1M-0</option>
		</select>
	      </td>
	      <td>
		<input type="NUMBER" name="ROOM[{$idx}]" class="txt3" min="0" max="127" size="3" maxlength="3" value="1">/
		<input type="NUMBER" name="REGION[{$idx}]" class="txt3" min="0" max="127" size="3" maxlength="3" value="1">/
		<input type="NUMBER" name="ORD[{$idx}]" class="txt5" min="0" max="30000" size="5" maxlength="5" value="1">/
		<input type="NUMBER" name="PRIORITY[{$idx}]" class="txt3" min="0" max="30" size="2" maxlength="2" value="15">
	    </tr>
	    <tr>
	      <td><input type="TEXT" name="CCMTYPE[{$idx}]" size="20" maxlength="20" value=""></td>
	      <td colspan="3">出力内容</td>
	    </tr>
	  {/for}
	  </table>
	</div>
	<h2>受信CCM情報設定</h2>
	<div class="d02 ws-padding">
	  <table class="t05">
	    <tr>
	      <th rowspan="2">No.</th>
	      <th width="400px">CCM名称</th>
	      <th>桁数</th>
	      <th>単位</th>
	      <th>Lev</th>
	      <th>ROOM/REGION/ORDER/PRIORITY</th>
	    </tr>
	    <tr>
	      <th colspan="2">反映時間帯</th>
	      <th colspan="3"></th>
	    </tr>
	  {for $idx=1 to 7}
	    <tr>
	      <td rowspan="2">{$idx}</td>
	      <td>
		<input type="CHECKBOX" name="TIMED[{$idx}]">
		<input type="NUMBER" name="STHR[{$idx}]" class="txt3" min="0" max="23" size="2" maxlength="2" value="0">:
		<input type="NUMBER" name="STMN[{$idx}]" class="txt3" min="0" max="59" size="2" maxlength="2" value="0">&#126;
		<input type="NUMBER" name="EDHR[{$idx}]" class="txt3" min="0" max="23" size="2" maxlength="2" value="0">:
		<input type="NUMBER" name="EDMN[{$idx}]" class="txt3" min="0" max="59" size="2" maxlength="2" value="0">
		&nbsp;
		<input type="NUMBER" name="EDHR[{$idx}]" class="txt3" min="0" max="23" size="2" maxlength="2" value="0">分間隔
		<input type="NUMBER" name="EDMN[{$idx}]" class="txt3" min="0" max="59" size="2" maxlength="2" value="0">分間
	      </td>
	      <td><input type="NUMBER" name="CAST[{$idx}]" class="txt3" size="2" min="0" max="18" value="0"></td>
	      <td><input type="TEXT" name="UNIT[{$idx}]" size="8" placeholder="単位" value=""></td>
	      <td>
		<select name="RLev[{$idx}]">
		  <option>A-1S-0</option>
		  <option>A-1S-1</option>
		  <option>A-10S-0</option>
		  <option>A-10S-1</option>
		  <option>A-1M-0</option>
		  <option>A-1M-1</option>
		  <option>B-0</option>
		  <option>B-1</option>
		  <option>S-1S-0</option>
		  <option>S-1M-0</option>
		</select>
	      </td>
	      <td>
		<input type="NUMBER" name="ROOM[{$idx}]" class="txt3" min="0" max="127" size="3" maxlength="3" value="1">/
		<input type="NUMBER" name="REGION[{$idx}]" class="txt3" min="0" max="127" size="3" maxlength="3" value="1">/
		<input type="NUMBER" name="ORD[{$idx}]" class="txt5" min="0" max="30000" size="5" maxlength="5" value="1">/
		<input type="NUMBER" name="PRIORITY[{$idx}]" class="txt3" min="0" max="30" size="2" maxlength="2" value="15">
	      </td>
	    </tr>
	    <tr>
	      <td><input type="TEXT" name="CCMTYPE[{$idx}]" size="20" value=""></td>
	      <td colspan="4">
	      </td>
	    </tr>
	    {/for}
	  </table>
	</div>
	<div id="m-btn">
	  <input type="SUBMIT" name="EXECMODE" value="Build">
	  <input type="SUBMIT" name="EXECMODE" value="arraydump">
	  <input type="RESET" name="EXECMODE" value="Cancel">
	</div>
      </form>
      <script>
	function setIpChange(event){
	    if (DHCPFid.checked){
		FIXIPid.disabled = true;
		BITMASKid.disabled = true;
		GATEWAYid.disabled = true;
		DNSid.disabled = true;
	    }else{
		FIXIPid.disabled = false;
		BITMASKid.disabled = false;
		GATEWAYid.disabled = false;
		DNSid.disabled = false;
	    }
	}
	let DHCPFid = document.getElementById('DHCPFid');
	DHCPFid.addEventListener('change', setIpChange);
      </script>
  </body>
</html>
