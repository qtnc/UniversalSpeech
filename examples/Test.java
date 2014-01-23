import static quentinc.UniversalSpeech.*;

public class Test {
public static void main (String[] args) throws Exception {
setValue(ENABLE_NATIVE_SPEECH, true);

System.out.println("Here is a list of supported engines : ");
System.out.printf("%-6s. %-50s, %10s%n", "ID", "Name", "Available");

String name;
for (int i=0; (name = getString(ENGINE+i))!=null; i++) {
boolean avail = getValue(ENGINE_AVAILABLE+i)!=0;
System.out.printf("%6d. %-50s, %-10s%n", i, name, avail?"yes":"no");
}

while(true){
say("You are using " + getString(ENGINE+getValue(ENGINE)));
Thread.currentThread().sleep(3000);
}

}}