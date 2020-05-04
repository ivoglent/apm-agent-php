<?php

declare(strict_types=1);

namespace Elastic\Apm\Impl\AutoInstrument;

use Elastic\Apm\AutoInstrument\RegistrationContextInterface;
use Elastic\Apm\Impl\AutoInstrument\Pdo\PdoAutoInstrumentation;

/**
 * Code in this file is part of implementation internals and thus it is not covered by the backward compatibility.
 *
 * @internal
 */
final class BuiltinAutoInstrumentations
{
    public static function register(RegistrationContextInterface $ctx): void
    {
        PdoAutoInstrumentation::register($ctx);
    }
}
