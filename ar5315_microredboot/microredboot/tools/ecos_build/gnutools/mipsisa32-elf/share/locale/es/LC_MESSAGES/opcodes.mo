��    W      �     �      �  [   �     �  9   �  (   3     \     i  	   y     �  	   �     �     �     �  (   �     	     -	     M	     h	     �	     �	     �	     �	     �	  ,   �	     
  :   6
  1   q
  9   �
  6   �
       "   .  )   Q     {  %   �  #   �  +   �  +     1   /  1   a  %   �  +   �  1   �  1     /   I     y     �     �     �  .   �  +   	     5     O  !   j  5   �  "   �  0   �          &     F  )   c     �     �  %   �  &   �          &  !   :     \  !   w  !   �  2   �  2   �  2   !  0   T  -   �  ;   �  	   �     �                     /     G     b      w     �     �  5  �  p   �  !   k  B   �  ,   �     �             -   .     \     j     �  "   �  ;   �     �  -        ;     Z  %   v  $   �     �     �     �  2   �      ,  =   M  4   �  <   �  <   �  %   :  6   `  ,   �     �  +   �  2     /   9  1   i  8   �  8   �  7     -   E  ;   s  ;   �  C   �     /     K  $   j  &   �  J   �  J     ,   L  !   y  )   �  ?   �  ,     G   2     z  &   �     �  /   �  $        -  +   I  5   u     �     �  *   �  (     ,   6  ,   c  5   �  5   �  5   �  3   2   A   f   W   �       !     !     !     +!     ?!     R!  +   o!     �!  "   �!     �!     �!     P   ;   J          >       ?   I           D   H   -      .   2          :   <   !           F   5   "            7   V           8          4            1      T   *   K       %                    Q                 L   U   )   &           ,      +   
           S   3      C                      G       	                 B   /                  A          #   =   @       $          O      E       6   R       M           (              0   N   9   W       '                  
The following ARM specific disassembler options are supported for use with
the -M switch:
 # <dis error: %08x> # internal disassembler error, unrecognised modifier (%c) # internal error, undefined modifier(%c) $<undefined> %02x		*unknown* (unknown) *unknown operands type: %d* *unknown* <function code %d> <illegal precision> <internal disassembler error> <internal error in opcode table: %s %s>
 <unknown register %d> Address 0x%x is out of bounds.
 Bad case %d (%s) in %s:%d
 Bad immediate expression Bad register in postincrement Bad register in preincrement Bad register name Don't understand %x 
 Hmmmm %x Illegal limm reference in last instruction!
 Internal disassembler error Internal error:  bad sparc-opcode.h: "%s", %#.8lx, %#.8lx
 Internal error: bad sparc-opcode.h: "%s" == "%s"
 Internal error: bad sparc-opcode.h: "%s", %#.8lx, %#.8lx
 Internal: Non-debugged code (test-case missing): %s:%d Label conflicts with `Rx' Label conflicts with register name Small operand was not an immediate number Unknown error %d
 Unrecognised disassembler option: %s
 Unrecognised register name set: %s
 Unrecognized field %d while building insn.
 Unrecognized field %d while decoding insn.
 Unrecognized field %d while getting int operand.
 Unrecognized field %d while getting vma operand.
 Unrecognized field %d while parsing.
 Unrecognized field %d while printing insn.
 Unrecognized field %d while setting int operand.
 Unrecognized field %d while setting vma operand.
 attempt to set y bit when using + or - modifier bad instruction `%.50s' bad instruction `%.50s...' branch operand unaligned branch to odd offset branch value not in range and to an odd offset branch value not in range and to odd offset branch value out of range can't cope with insert %d
 displacement value is not aligned displacement value is not in range and is not aligned displacement value is out of range ignoring least significant bits in branch offset illegal bitmask immediate value is out of range immediate value must be even immediate value not in range and not even index register in load range invalid conditional option invalid register for stack adjustment invalid register operand when updating jump hint unaligned junk at end of line missing mnemonic in syntax string offset not a multiple of 4 offset not between -2048 and 2047 offset not between -8192 and 8191 operand out of range (%ld not between %ld and %ld) operand out of range (%ld not between %ld and %lu) operand out of range (%lu not between %lu and %lu) operand out of range (%lu not between 0 and %lu) syntax error (expected char `%c', found `%c') syntax error (expected char `%c', found end of instruction) undefined unknown unknown	0x%02x unknown	0x%04lx unknown	0x%04x unknown constraint `%c' unknown operand shift: %x
 unknown pop reg: %d
 unrecognized form of instruction unrecognized instruction value out of range Project-Id-Version: opcodes 2.12.91
POT-Creation-Date: 2002-07-23 15:55-0400
PO-Revision-Date: 2002-07-24 02:03-0500
Last-Translator: Cristian Oth�n Mart�nez Vera <cfuga@itam.mx>
Language-Team: Spanish <es@li.org>
MIME-Version: 1.0
Content-Type: text/plain; charset=ISO-8859-1
Content-Transfer-Encoding: 8bit
 
Las siguientes opciones de desensamblador espec�ficas de ARM tienen soporte
para su uso con el interruptor -M:
 # <error de desensamblador: %08x> # error interno del desensamblador, modificador (%c) no reconocido # error interno, modificador(%c) sin definir $<sin definir> %02x		*desconocido* (desconocido) *tipo de operandos operandos desconocido: %d* *desconocida* <c�digo de funci�n %d> <precisi�n ilegal> <error interno del desensamblador> <error interno en la tabla de c�digos de operaci�n: %s %s>
 <registro %d desconocido> La direcci�n 0x%x est� fuera de los l�mites.
 Case %d err�neo (%s) en %s:%d
 Expresi�n inmediata err�nea Registro err�neo en el postincremento Registro err�neo en el preincremento Nombre de registro err�neo No se entiende %x 
 Hmmmm %x �Referencia limm ilegal en la �ltima instrucci�n!
 Error interno del desensamblador Error interno:  sparc-opcode.h err�neo: "%s", %#.8lx, %#.8lx
 Error interno: sparc-opcode.h err�neo: "%s" == "%s"
 Error interno: sparc-opcode.h err�neo: "%s", %#.8lx, %#.8lx
 Interno: C�digo no depurado (falta el caso de prueba): %s:%d La etiqueta tiene conflictos con `Rx' La etiqueta tiene conflictos con el nombre de registro El operando small no era un n�mero inmediato Error desconocido %d
 Opci�n de desensamblador no reconocida: %s
 Conjunto de nombres de registro no reconocido: %s
 No se reconoci� el campo %d al construir insn.
 No se reconoci� el campo %d al decodificar insn.
 No se reconoci� el campo %d al obtener el operando int.
 No se reconoci� el campo %d al obtener el operando vma.
 No se reconoci� el campo %d durante la decodificaci�n.
 No se reconoci� el campo %d al mostrar insn.
 No se reconoci� el campo %d al establecer el operando int.
 No se reconoci� el campo %d al establecer el operando vma.
 intento de establecer el bit y cuando se usaba el modificador + � - instrucci�n err�nea `%.50s' instrucci�n err�nea `%.50s...' operando de ramificaci�n sin alinear ramificaci�n a un desplazamiento impar el valor de ramificaci�n no est� en rango e indica un desplazamiento impar el valor de ramificaci�n no est� en rango e indica un desplazamiento impar el valor de ramificaci�n est� fuera de rango no se puede lidiar con insert %d
 el valor de desubicaci�n no est� alineado el valor de desubicaci�n no est� en el rango y no est� alineado el valor de desubicaci�n est� fuera de rango ignorando los bits menos significativos en el desplazamiento de la rama m�scara de bits ilegal el valor inmediato est� fuera de rango el valor inmediato debe ser par el valor inmediato no est� en rango y no es par registro �ndice en el rango de carga opci�n condicional inv�lida registro inv�lido para el ajuste de la pila operando de registro inv�lido mientras se actualizaba pista de salto sin alinear basura al final de la l�nea falta el mnem�nico en la cadena sint�ctica el desplazamiento no es un m�ltiplo de 4 el desplazamiento no est� entre -2048 y 2047 el desplazamiento no est� entre -8192 y 8191 operando fuera de rango (%ld no est� entre %ld y %ld) operando fuera de rango (%ld no est� entre %ld y %lu) operando fuera de rango (%lu no est� entre %lu y %lu) operando fuera de rango (%lu no est� entre 0 y %lu) error sint�ctico (se esperaba el car�cter `%c', se encontr� `%c') error sint�ctico (se esperaba el car�cter `%c', se encontr� el final de la instrucci�n) sin definir desconocida desconocido	0x%02x desconocido	0x%04lx desconocido	0x%04x restricci�n `%c' desconocida operando de desplazamiento desconocido: %x
 registro pop desconocido: %d
 forma de instrucci�n no reconocida instrucci�n no reconocida valor fuera de rango 