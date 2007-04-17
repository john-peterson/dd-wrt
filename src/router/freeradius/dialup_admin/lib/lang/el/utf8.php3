<?php
function init_encoder()
{
	$k = array(

		"~@"=>"", "~A"=>"", "~B"=>"", "~C"=>"", "~D"=>"", "~E"=>"",
		"~F"=>"", "~G"=>"", "~H"=>"", "~I"=>"", "~J"=>"", "~K"=>"",
		"~L"=>"", "~M"=>"", "~N"=>"", "~O"=>"", "~P"=>"", "~Q"=>"",
		"~R"=>"", "~S"=>"", "~T"=>"", "~U"=>"", "~V"=>"", "~W"=>"",
		"~^"=>"", "~_"=>"", "| "=>" ", "�"=>"ʽ", "�"=>"ʼ", "�"=>"£",
		"�"=>"�", "�"=>"�", "�"=>"¦", "�"=>"§", "�"=>"¨", "�"=>"©",
		"�"=>"�", "�"=>"«", "�"=>"¬", "�"=>"­", "�"=>"�", "�"=>"―",
		"�"=>"°", "�"=>"±", "�"=>"²", "�"=>"³", "�"=>"΄", "�"=>"΅",
		"�"=>"Ά", "�"=>"·", "�"=>"Έ", "�"=>"Ή", "�"=>"Ί", "�"=>"»",
		"�"=>"Ό", "�"=>"½", "�"=>"Ύ", "�"=>"Ώ", "�"=>"ΐ", "�"=>"Α",
		"�"=>"Β", "�"=>"Γ", "�"=>"Δ", "�"=>"Ε", "�"=>"Ζ", "�"=>"Η",
		"�"=>"Θ", "�"=>"Ι", "�"=>"Κ", "�"=>"Λ", "�"=>"Μ", "�"=>"Ν",
		"�"=>"Ξ", "�"=>"Ο", "�"=>"Π", "�"=>"Ρ", "�"=>"�", "�"=>"Σ",
		"�"=>"Τ", "�"=>"Υ", "�"=>"Φ", "�"=>"Χ", "�"=>"Ψ", "�"=>"Ω",
		"�"=>"Ϊ", "�"=>"Ϋ", "�"=>"ά", "�"=>"έ", "�"=>"ή", "�"=>"ί",
		"�"=>"ΰ", "�"=>"α", "�"=>"β", "�"=>"γ", "�"=>"δ", "�"=>"ε",
		"�"=>"ζ", "�"=>"η", "�"=>"θ", "�"=>"ι", "�"=>"κ", "�"=>"λ",
		"�"=>"μ", "�"=>"ν", "�"=>"ξ", "�"=>"ο", "�"=>"π", "�"=>"ρ",
		"�"=>"ς", "�"=>"σ", "�"=>"τ", "�"=>"υ", "�"=>"φ", "�"=>"χ",
		"�"=>"ψ", "�"=>"ω", "�"=>"ϊ", "�"=>"ϋ", "�"=>"ό", "�"=>"ύ",
		"�"=>"ώ", "~X"=>"", "~Y"=>"", "~Z"=>"", "~["=>"", "~]"=>""
	);

	return $k;
}

function init_decoder()
{
	$k = array(
		""=>"~@", ""=>"~A", ""=>"~B", ""=>"~C", ""=>"~D", ""=>"~E",
		""=>"~F", ""=>"~G", ""=>"~H", ""=>"~I", ""=>"~J", ""=>"~K",
		""=>"~L", ""=>"~M", ""=>"~N", ""=>"~O", ""=>"~P", ""=>"~Q",
		""=>"~R", ""=>"~S", ""=>"~T", ""=>"~U", ""=>"~V", ""=>"~W",
		""=>"~^", ""=>"~_", " "=>"| ", "ʽ"=>"�", "ʼ"=>"�", "£"=>"�",
		"�"=>"�", "�"=>"�", "¦"=>"�", "§"=>"�", "¨"=>"�", "©"=>"�",
		"�"=>"�", "«"=>"�", "¬"=>"�", "­"=>"�", "�"=>"�", "―"=>"�",
		"°"=>"�", "±"=>"�", "²"=>"�", "³"=>"�", "΄"=>"�", "΅"=>"�",
		"Ά"=>"�", "·"=>"�", "Έ"=>"�", "Ή"=>"�", "Ί"=>"�", "»"=>"�",
		"Ό"=>"�", "½"=>"�", "Ύ"=>"�", "Ώ"=>"�", "ΐ"=>"�", "Α"=>"�",
		"Β"=>"�", "Γ"=>"�", "Δ"=>"�", "Ε"=>"�", "Ζ"=>"�", "Η"=>"�",
		"Θ"=>"�", "Ι"=>"�", "Κ"=>"�", "Λ"=>"�", "Μ"=>"�", "Ν"=>"�",
		"Ξ"=>"�", "Ο"=>"�", "Π"=>"�", "Ρ"=>"�", "�"=>"�", "Σ"=>"�",
		"Τ"=>"�", "Υ"=>"�", "Φ"=>"�", "Χ"=>"�", "Ψ"=>"�", "Ω"=>"�",
		"Ϊ"=>"�", "Ϋ"=>"�", "ά"=>"�", "έ"=>"�", "ή"=>"�", "ί"=>"�",
		"ΰ"=>"�", "α"=>"�", "β"=>"�", "γ"=>"�", "δ"=>"�", "ε"=>"�",
		"ζ"=>"�", "η"=>"�", "θ"=>"�", "ι"=>"�", "κ"=>"�", "λ"=>"�",
		"μ"=>"�", "ν"=>"�", "ξ"=>"�", "ο"=>"�", "π"=>"�", "ρ"=>"�",
		"ς"=>"�", "σ"=>"�", "τ"=>"�", "υ"=>"�", "φ"=>"�", "χ"=>"�",
		"ψ"=>"�", "ω"=>"�", "ϊ"=>"�", "ϋ"=>"�", "ό"=>"�", "ύ"=>"�",
		"ώ"=>"�", ""=>"~X", ""=>"~Y", ""=>"~Z", ""=>"~[", ""=>"~]"
	);

	return $k;
}

function encode_string($line,$k)
{
	for($i=0;$i<strlen($line);$i++){
		$c = $line{$i};
		$val = $k["$c"];
		$c = ($val != "") ? "$val" : "$c";
		$new_line .= $c;
	}

	return $new_line;
}

function decode_string($line,$k)
{
	$line = ereg_replace("&","&&",$line);
	$line = ereg_replace("([,+0-9./() -])", "%\\1", $line);
	$mline = chunk_split($line, 2, " ");
	$chars = explode(" ", $mline);
	foreach ($chars as $c){
		$val = $k["$c"];
		$c = ($val != "") ? "$val" : "$c";
		$new_line .= $c;
	}
	$new_line = ereg_replace("%%", " ", $new_line);
	$new_line = ereg_replace("%([,+0-9./() -])", "\\1", $new_line);
	$new_line = ereg_replace("%", " ",$new_line);
	$new_line = ereg_replace("&&","&",$new_line);

	return $new_line;
}
?>
