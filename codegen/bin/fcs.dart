import 'libs.dart' as libs;
import 'apps.dart' as apps;
import 'schemas.dart' as schemas;
import '../lib/installation.dart';


main() {
  libs.addItems();
  apps.addItems();
  schemas.addItems()
    .then((var _) {
      installation.generate(true);
      print('Installed libs ${installation.generatedLibs.map((l) => l.id)}');
      print('Installed apps ${installation.generatedApps.map((l) => l.id)}');
    });
}
