<!DOCTYPE html>
<html lang="ja">
	<head>
		<meta charset="utf-8">
		<title>M304 parameter compiler</title>
		<meta name="description" content="https://at.strix-inc.jp/8487/">
		<meta name="keywords" content="M304">
		<meta name="viewport" content="width=device-width,initial-scale=1.0">
		<meta name="format-detection" content="telephone=no,email=no,address=no">

		<!-- Import Google Fonts -->
		<link rel="preconnect" href="https://fonts.googleapis.com">
		<link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
		<link href="https://fonts.googleapis.com/css2?family=Noto+Sans+JP:wght@400;500;700&display=swap" rel="stylesheet">

		<!-- Import CSS -->
		<link rel="stylesheet" href="css/style.css">

		<!-- Import JS/jQuery Library -->
		<script src="https://code.jquery.com/jquery-1.12.4.min.js"></script>
	</head>
	<body>
		<h1>IntelHEX出力</h1>
		<p>{$msg}</p>
		<table>
			<thead>
			</thead>
			<tbody>
				<tr>
					<td style="text-align: left;">
						<pre>
{foreach $IHEXDATA as $row}
{$row}
{/foreach}
						</pre>
					</td>
				</tr>
			</tbody>
		</table>
	</body>
</html>
