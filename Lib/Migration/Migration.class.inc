<?php
namespace Lib\Migration;

/**
 * Description of Migration
 *
 * @author bryyce
 */
abstract class Migration {
    public abstract function up();
    public abstract function down();
    
}
