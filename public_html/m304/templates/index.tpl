<!DOCTYPE html>
<html lang="ja">
  <head>
    <meta charset="utf-8">
    <title>UECS for M304</title>
    <meta version="3.0.0" driver_version="3.0.0">
    <link href="css/main.css" rel="stylesheet" type="text/css">
  </head>
  <body id="tools">
    <div id="t-header">
      <a name="pagetop"></a>
      <h1 id="tools">M304 builder Version:{$VERSION}</h1>
    </div>
    <div id="wrapper">
      <form action="index.php" method="POST">
      <h2>NODE INFO  <input type="checkbox" name="DBGF"></h2>
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
	    <input type="HIDDEN" name="ihex" value="{$ihex}">
	    {if $ihex!="NON"}
	    <tr>
	      <td colspan="2">
		変換結果：
<pre>
{$ihex}
</pre>
	      </td>
	    </tr>
	    {/if}
	    {if $DBG}
	    <tr>
	      <td colspan="2">デバッグ結果 EXECMODE:{$EXECM}
		<ul>
		{foreach from=$INFOMETHOD key=k item=v}
		<li>{$k}: {$v}</li>
		{/foreach}
	      </td>
	    </tr>
	    {/if}
	    <tr>
	      <td>
		<input type="SUBMIT" name="EXECMODE" value="NodeInfo Build">
		<input type="RESET" name="EXECMODE" value="Cancel">
	      </td>
	    </tr>
	  </table>
	</div>
	<h2>動作条件情報設定</h2>
	<div class="d02 ws-padding">
	  <table class="t05">
	    {include file="rx_part.tpl"}
	    <tr>
	      <td colspan="6">
		<input type="SUBMIT" name="EXECMODE" value="RX Build">
		<input type="RESET" name="EXECMODE" value="Cancel">
	      </td>
	    </tr>
	  </table>
	</div>
	<h2>比較条件設定  <input type="checkbox" name="DBGF"></h2>
	<div class="d02 ws-padding">
	  <table class="t05">
	    {include file="ope_part.tpl"}
	    <tr>
	      <td colspan="4">
		<input type="SUBMIT" name="EXECMODE" value="Ope Build">
		<input type="RESET" name="EXECMODE" value="Cancel">
	      </td>
	    </tr>
	    {if $DBG}
	    <tr>
	      <td colspan="4">デバッグ結果 EXECMODE:{$EXECM}
		<ul>
		  {for $o=0 to 10}
		  {foreach from=$OPEMETHOD[$o] key=k item=v}
		<li>{$o}: {$k}: {$v}</li>
		{/foreach}
		{/for}
		</ul>
	      </td>
	    </tr>
	    {/if}
	  </table>
	</div>
	<h2>送信CCM情報設定  <input type="checkbox" name="DBGF"></h2>
	<div class="d02 ws-padding">
	  <table class="t05">
	    {include file="tx_part.tpl"}
	    {if $DBG}
	    <tr>
	      <td colspan="6">デバッグ結果 EXECMODE:{$EXECM}
		<ul>
		  {for $o=0 to 10}
		  {foreach from=$TXMETHOD[$o] key=k item=v}
		<li>{$o}: {$k}: {$v}</li>
		{/foreach}
		{/for}
		</ul>
	      </td>
	    </tr>
	    {/if}
	    <tr>
	      <td colspan="6">
		<input type="SUBMIT" name="EXECMODE" value="TX Build">
		<input type="RESET" name="EXECMODE" value="Cancel">
	      </td>
	    </tr>
	  </table>
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
